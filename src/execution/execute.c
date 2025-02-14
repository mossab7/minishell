#include <zen.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>


// Structure to track pipe file descriptors
typedef struct s_pipe_info {
    int read_fd;
    int write_fd;
    bool is_active;
} t_pipe_info;
int execute_ast(t_ast *node, t_pipe_info *pipe_info);

// Helper function to set up redirections
static int setup_redirections(t_command *cmd) {
    for (int i = 0; i < cmd->redirect_count; i++) {
        t_redirect *redir = cmd->redirects[i];
        int fd;

        switch (redir->type) {
            case REDIR_INPUT:
                fd = open(redir->filename, O_RDONLY);
                if (fd == -1) {
                    perror("open");
                    return -1;
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                break;

            case REDIR_OUTPUT:
                fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("open");
                    return -1;
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                break;

            case REDIR_APPEND:
                fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd == -1) {
                    perror("open");
                    return -1;
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                break;

            case REDIR_HEREDOC:
                // Create a temporary file for heredoc
                char template[] = "/tmp/heredoc_XXXXXX";
                fd = mkstemp(template);
                if (fd == -1) {
                    perror("mkstemp");
                    return -1;
                }
                
                // Write heredoc content to temp file
                write(fd, redir->delimiter, strlen(redir->delimiter));
                write(fd, "\n", 1);
                
                // Reset file position to start
                lseek(fd, 0, SEEK_SET);
                
                // Redirect stdin from temp file
                dup2(fd, STDIN_FILENO);
                close(fd);
                unlink(template);  // Delete temp file
                break;
        }
    }
    return 0;
}

// Execute a single command
static int execute_command(t_command *cmd) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {  // Child process
        if (setup_redirections(cmd) == -1) {
            exit(EXIT_FAILURE);
        }

        execvp(cmd->args[0], cmd->args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Parent process
    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
}

// Execute a pipeline of commands
static int execute_pipe(t_ast *node, t_pipe_info *pipe_info) {
    if (node == NULL) return 0;

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    // Save current pipe info
    t_pipe_info current_pipe = {
        .read_fd = pipefd[0],
        .write_fd = pipefd[1],
        .is_active = true
    };

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {  // Child process
        close(pipefd[0]);  // Close read end
        
        // Set up stdout to write to pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // If there's an active input pipe, set it up
        if (pipe_info && pipe_info->is_active) {
            dup2(pipe_info->read_fd, STDIN_FILENO);
            close(pipe_info->read_fd);
            close(pipe_info->write_fd);
        }

        int result = execute_ast(node->left, NULL);
        exit(result);
    }

    // Parent process
    close(pipefd[1]);  // Close write end

    if (pipe_info && pipe_info->is_active) {
        close(pipe_info->read_fd);
        close(pipe_info->write_fd);
    }

    // Execute right side of pipe with new pipe info
    int right_result = execute_ast(node->right, &current_pipe);

    // Wait for left side to complete
    int status;
    waitpid(pid, &status, 0);
    int left_result = WEXITSTATUS(status);

    return (left_result == 0 && right_result == 0) ? 0 : 1;
}

// Execute a subshell
static int execute_subshell(t_ast *node) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {  // Child process
        int result = execute_ast(node->left, NULL);
        exit(result);
    }

    // Parent process
    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
}

// Main execution function
int execute_ast(t_ast *node, t_pipe_info *pipe_info) {
    if (node == NULL) return 0;

    switch (node->type) {
        case NODE_COMMAND:
            return execute_command(&node->value.command);

        case NODE_PIPE:
            return execute_pipe(node, pipe_info);

        case NODE_SUBSHELL:
            return execute_subshell(node);

        case NODE_LOGICAL_AND: {
            int left_result = execute_ast(node->left, pipe_info);
            if (left_result == 0) {
                return execute_ast(node->right, pipe_info);
            }
            return left_result;
        }

        case NODE_LOGICAL_OR: {
            int left_result = execute_ast(node->left, pipe_info);
            if (left_result != 0) {
                return execute_ast(node->right, pipe_info);
            }
            return left_result;
        }

        default:
            fprintf(stderr, "Unknown node type\n");
            return -1;
    }
}

// Main entry point for execution
int execute_command_line(t_ast *ast) {
    // Save original stdin/stdout
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

    // Execute the AST
    int result = execute_ast(ast, NULL);

    // Restore original stdin/stdout
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);

    return result;
}