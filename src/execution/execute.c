#include "zen.h"
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include "signals.h"

int execute_here_doc(t_redirect *redir)
{
	int fd;

	fd = open(redir->filename, O_RDONLY);
	unlink(redir->filename);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int inptu_redirection(t_redirect *redir)
{
	int fd;
	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (fd);
}

int output_redirection(t_redirect *redir)
{
	int fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (fd);
}

int append_redirection(t_redirect *redir)
{
	int fd;
	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (fd);
}

bool is_ambiguous_redirect(const char *filename) 
{
    struct stat file_stat;
    
    if (!filename || filename[0] == '\0')
        return true;
    
    if (stat(filename, &file_stat) == 0) 
	{
        if (S_ISDIR(file_stat.st_mode)) 
		{
            zen_elog("bash: %s/: Is a directory\n", filename);
            return true;
        }
    }
    
    if (strchr(filename, '$') || strchr(filename, '*') || strchr(filename, '?'))
        return true;
    
    return false;
}

int	setup_redirections(t_command *cmd)
{
	t_redirect	*redir;
	int fd;

	for (int i = 0; i < cmd->redirect_count; i++)
	{
		redir = cmd->redirects[i];
		if (redir->type != REDIR_HEREDOC) 
		{
            if (is_ambiguous_redirect(redir->filename)) 
			{
                zen_elog("ambiguous redirect\n");
                return (-1);
            }
        }
		if (redir->type == REDIR_INPUT)
		{
			fd = inptu_redirection(redir);
			if(fd == -1)
				return (-1);
		}
		else if(redir->type == REDIR_OUTPUT)
		{
			fd = output_redirection(redir);
			if(fd == -1)
				return (-1);
		}
		else if(redir->type == REDIR_APPEND)
		{
			fd = append_redirection(redir);
			if(fd == -1)
				return (-1);
		}
		else if(redir->type == REDIR_HEREDOC)
		{
			fd = execute_here_doc(redir);
			if(fd == -1)
				return (-1);
		}
	}
	return (0);
}

typedef int	(*built_in_command)(t_env *, char **args);
// TODO exit status of build in commands
// TODO add unset and set
int	execute_built_in_commands(t_command *cmd,char *command, t_env *env, char **args)
{
	built_in_command	functions[] = {built_in_cd, built_in_echo, built_in_env,
			built_in_exit, built_in_export, built_in_set, built_in_unset, NULL};
	char				*function_names[] = {"cd", "echo", "env", "exit",
						"export", "set", "unset", NULL};
	int					i;
	int					original_stdin;
	int					original_stdout;
	int					redirection_result;
	int					command_result;

	i = 0;
	while (function_names[i])
	{
		if (ft_strcmp(function_names[i], command) == 0)
			break ;
		i++;
	}
	if (!functions[i])
		return (1);
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdin == -1 || original_stdout == -1)
	{
		perror("dup");
		return (1);
	}
	redirection_result = setup_redirections(cmd);
	if (redirection_result == -1)
	{
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdin);
		close(original_stdout);
		return (1);
	}
	command_result = functions[i](env, args);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	return (command_result);
}

int	execute_command(t_command *cmd, t_env *env)
{
	int			status;
	pid_t		current_pid;
	t_string	*cmd_path;
	t_context	*context;

	context = *get_context();
	status = 0;
	if (cmd->argc == 0)
		return (0);
	if (!execute_built_in_commands(cmd,cmd->args[0], env, cmd->args))
		return (0);
	current_pid = fork();
	if (current_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (current_pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (setup_redirections(cmd) == -1)
			exit(EXIT_FAILURE);
		cmd_path = search_path(env->path, cmd->args[0]);
		if (cmd_path)
		{
			execve(cmd_path->cstring, cmd->args, env->envp);
			str_destruct(cmd_path);
		}
		ft_printf("minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	waitpid(current_pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		env->last_command_status = 128 + WTERMSIG(status);
		if(WTERMSIG(status) == SIGQUIT)
			ft_printf("Quit (core dumped)\n");
		else if(WTERMSIG(status) == SIGINT)
			context->siginit_received = true;
	}
	else
	{
		env->last_command_status = WEXITSTATUS(status);
	}
	return (env->last_command_status);
}

int	execute_pipe(t_ast *node, t_env *env)
{
	int	pipefd[2];
	int	status;
	int	pipe_status;

	pid_t pid1, pid2;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute_ast(node->left, env));
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execute_ast(node->right, env));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &pipe_status, 0);
	env->last_command_status = WEXITSTATUS(pipe_status);
	return (env->last_command_status);
}

int	execute_subshell(t_ast *node, t_env *env)
{
	pid_t	pid;
	t_env	*subshell_env;
	int		result;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		subshell_env = env_copy(env);
		result = execute_ast(node->left, subshell_env);
		env_destroy(subshell_env);
		exit(result);
	}
	waitpid(pid, &status, 0);
	env->last_command_status = WEXITSTATUS(status);
	return (env->last_command_status);
}

int	execute_ast(t_ast *node, t_env *env)
{
	int	left_status;

	if (!node)
		return (0);

	if  (node->type == NODE_COMMAND)
		return (execute_command(&node->value.command, env));
	else if (node->type == NODE_PIPE)
		return (execute_pipe(node, env));
	else if (node->type == NODE_SUBSHELL)
		return execute_subshell(node, env);
	else if (node->type == NODE_LOGICAL_AND)
	{
		left_status = execute_ast(node->left, env);
		if (left_status == 0)
			return execute_ast(node->right, env);
		return left_status;
	}
	else if (node->type == NODE_LOGICAL_OR)
	{
		left_status = execute_ast(node->left, env);
		if (left_status != 0)
			return execute_ast(node->right, env);
		return left_status;
	}
	else
	{
		ft_printf("Unknown node type\n");
		return -1;
	}
}
