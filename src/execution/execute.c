#include "zen.h"
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>

int setup_redirections(t_command *cmd, int *saved_fds)
{
    saved_fds[0] = dup(STDIN_FILENO);
    saved_fds[1] = dup(STDOUT_FILENO);

    for (int i = 0; i < cmd->redirect_count; i++) {
        t_redirect *redir = cmd->redirects[i];
        int fd;

        switch (redir->type) {
            case REDIR_INPUT:
                fd = open(redir->filename, O_RDONLY);
                if (fd == -1) {
                    perror(redir->filename);
                    return -1;
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                break;

            case REDIR_OUTPUT:
                fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror(redir->filename);
                    return -1;
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                break;

            case REDIR_APPEND:
                fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd == -1) {
                    perror(redir->filename);
                    return -1;
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                break;

            case REDIR_HEREDOC:
                char template[] = "/tmp/heredoc_XXXXXX";
                fd = mkstemp(template);
                if (fd == -1) {
                    perror("heredoc");
                    return -1;
                }
                unlink(template);

                char *line;
                while ((line = readline("> ")) != NULL) {
                    if (strcmp(line, redir->delimiter) == 0) {
                        free(line);
                        break;
                    }
                    write(fd, line, strlen(line));
                    write(fd, "\n", 1);
                    free(line);
                }

                lseek(fd, 0, SEEK_SET);
                dup2(fd, STDIN_FILENO);
                close(fd);
                break;
        }
    }
    return 0;
}

int execute_command(t_command *cmd, t_env *env)
{
    int saved_fds[2];
    int status = 0;

    if (cmd->argc == 0)
		return 0;
    pid_t current_pid = fork();
	if (current_pid == -1)
	{
        perror("fork");
        return -1;
    }
    if (current_pid == 0)
	{
        if (setup_redirections(cmd, saved_fds) == -1)
            exit(EXIT_FAILURE);
        t_string *cmd_path = search_path(env->path, cmd->args[0]);
		if (cmd_path)
		{
            execve(cmd_path->cstring, cmd->args, env->envp);
            str_destruct(cmd_path);
        }
        ft_printf("minishell: %s: command not found\n", cmd->args[0]);
        exit(127);
    }

    waitpid(current_pid, &status, 0);
    if (WIFSIGNALED(status)) {
        env->last_command_status = 128 + WTERMSIG(status);
    } else {
        env->last_command_status = WEXITSTATUS(status);
    }

    return (env->last_command_status);
}

int execute_pipe(t_ast *node, t_env *env) {
    int pipefd[2];
    int status;
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        exit(execute_ast(node->left, env));
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (pid2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        exit(execute_ast(node->right, env));
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, &status, 0);
    int pipe_status;
    waitpid(pid2, &pipe_status, 0);

    env->last_command_status = WEXITSTATUS(pipe_status);
    return env->last_command_status;
}

int execute_subshell(t_ast *node, t_env *env)
{
    pid_t pid = fork();
    if (pid == -1)
	{
        perror("fork");
        return -1;
    }

    if (pid == 0)
	{
        t_env *subshell_env = env_copy(env);
        int result = execute_ast(node->left, subshell_env);
        env_destroy(subshell_env);
        exit(result);
    }

    int status;
    waitpid(pid, &status, 0);
    env->last_command_status = WEXITSTATUS(status);
    return env->last_command_status;
}

int execute_ast(t_ast *node, t_env *env) {
    if (!node) return 0;

    switch (node->type) {
        case NODE_COMMAND:
            return execute_command(&node->value.command, env);

        case NODE_PIPE:
            return execute_pipe(node, env);

        case NODE_SUBSHELL:
            return execute_subshell(node, env);

        case NODE_LOGICAL_AND: {
            int left_status = execute_ast(node->left, env);
            if (left_status == 0)
                return execute_ast(node->right, env);
            return left_status;
        }

        case NODE_LOGICAL_OR: {
            int left_status = execute_ast(node->left, env);
            if (left_status != 0)
                return execute_ast(node->right, env);
            return left_status;
        }

        default:
            ft_printf("Unknown node type\n");
            return -1;
    }
}

