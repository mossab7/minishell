/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:42:01 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 06:42:18 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	execute_here_doc(t_redirect *redir)
{
	int	fd;

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

int	inptu_redirection(t_redirect *redir)
{
	int	fd;

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

int	output_redirection(t_redirect *redir)
{
	int	fd;

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

int	append_redirection(t_redirect *redir)
{
	int	fd;

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

bool	is_ambiguous_redirect(const char *filename)
{
	struct stat	file_stat;

	if (!filename || filename[0] == '\0')
		return (true);
	if (stat(filename, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			zen_elog("bash: %s/: Is a directory\n", filename);
			return (true);
		}
	}
	if (strchr(filename, '$') || strchr(filename, '*') || strchr(filename, '?'))
		return (true);
	return (false);
}

int	setup_redirections(t_command *cmd)
{
	t_redirect	*redir;
	int			fd;
	int			i;

	i = 0;
	while(i < cmd->redirect_count)
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
		}
		else if (redir->type == REDIR_OUTPUT)
		{
			fd = output_redirection(redir);
		}
		else if (redir->type == REDIR_APPEND)
		{
			fd = append_redirection(redir);
		}
		else if (redir->type == REDIR_HEREDOC)
		{
			fd = execute_here_doc(redir);
		}
		if (fd == -1)
			return (-1);
		i++;
	}
	return (0);
}


int	setup_builtin_redirections(t_command *cmd, t_type type)
{
	static int					original_stdin;
	static int					original_stdout;
	int							redirection_result;

	if (type == setup)
	{
		original_stdin = dup(STDIN_FILENO);
		original_stdout = dup(STDOUT_FILENO);
		if (original_stdin == -1 || original_stdout == -1)
		{

			original_stdin = 0;
			original_stdout = 0;
			return ((perror("dup")),-1);
		}
		redirection_result = setup_redirections(cmd);
		if (redirection_result == -1)
		{
			dup2(original_stdin, STDIN_FILENO);
			dup2(original_stdout, STDOUT_FILENO);
			original_stdin = 0;
			original_stdout = 0;
			return ((close(original_stdin)),(close(original_stdout)),-1);
		}
	}
	else
	{
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdin);
		close(original_stdout);
		original_stdin = 0;
		original_stdout = 0;
	}
	return (0);
}

void	init_builtin_commands(built_in_command *functions, char **function_names)
{
	functions[0] = built_in_cd;
	functions[1] = built_in_echo;
	functions[2] = built_in_env;
	functions[3] = built_in_exit;
	functions[4] = built_in_export;
	functions[5] = built_in_set;
	functions[6] = built_in_unset;
	functions[7] = NULL;

	function_names[0] = "cd";
	function_names[1] = "echo";
	function_names[2] = "env";
	function_names[3] = "exit";
	function_names[4] = "export";
	function_names[5] = "set";
	function_names[6] = "unset";
	function_names[7] = NULL;
}


int	execute_built_in_commands(t_command *cmd, char *command, t_env *env,
		char **args)
{
	built_in_command	functions[BUILT_IN_COMMANDS_COUNT];
	char				*function_names[BUILT_IN_COMMANDS_COUNT];
	int					i;
	int					command_result;

	init_builtin_commands(functions, function_names);
	i = 0;
	while (function_names[i])
	{
		if (ft_strcmp(function_names[i], command) == 0)
			break ;
		i++;
	}
	if (!functions[i])
		return (-1);
	if (setup_builtin_redirections(cmd, setup) == -1)
		return (1);
	command_result = functions[i](env, args);
	setup_builtin_redirections(cmd, teardown);
	return (command_result);
}

void	launch_command(t_command *cmd, t_env *env)
{
	t_string	*cmd_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (setup_redirections(cmd) == -1)
		exit(EXIT_FAILURE);
	cmd_path = search_path(env->path, cmd->args[0]);
	if (cmd_path)
	{
		env_join(env);
		execve(cmd_path->cstring, cmd->args, env->envp);
		str_destruct(cmd_path);
	}
	ft_printf("minishell: %s: command not found\n", cmd->args[0]);
	exit(127);
}

int	get_command_status(t_env *env,int status)
{
	if (WIFSIGNALED(status))
	{
		env->last_command_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_printf("Quit (core dumped)\n");
		else if (WTERMSIG(status) == SIGINT)
			set_context_flag(FLAG_SIGINT_RECEIVED);
	}
	else
	{
		env->last_command_status = WEXITSTATUS(status);
	}
	return (env->last_command_status);
}

int	execute_command(t_command *cmd, t_env *env)
{
	int			status;
	pid_t		current_pid;

	status = 0;
	if (cmd->argc == 0)
		return (0);
	env->last_command_status = execute_built_in_commands(cmd, cmd->args[0], env,
			cmd->args);
	if (env->last_command_status != -1)
		return (env->last_command_status);
	current_pid = fork();
	if (current_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (current_pid == 0)
		launch_command(cmd,env);
	waitpid(current_pid, &status, 0);
	return (get_command_status(env,status));
}

int handle_pipe_error(int *pipefd, char *msg)
{
	perror(msg);
	if(pipefd)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (-1);
}

void launch_left_pipe(t_ast *node, t_env *env, int pipefd[2])
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_ast(node->left, env));
}

void launch_right_pipe(t_ast *node, t_env *env, int pipefd[2])
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_ast(node->right, env));
}
int	execute_pipe(t_ast *node, t_env *env)
{
	int	pipefd[2];
	int	pipe_status;
	pid_t pid1;
	pid_t pid2;

	if (pipe(pipefd) == -1)
		return (handle_pipe_error(NULL, "pipe"));
	pid1 = fork();
	if (pid1 == -1)
		return (handle_pipe_error(pipefd, "fork"));
	if (pid1 == 0)
		launch_left_pipe(node,env,pipefd);
	pid2 = fork();
	if (pid2 == -1)
		return (handle_pipe_error(pipefd, "fork"));
	if (pid2 == 0)
		launch_right_pipe(node,env,pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
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
		if (setup_redirections(&node->value.command) == -1)
		{
			env_destroy(subshell_env);
			exit(EXIT_FAILURE);
		}
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
	if (node->type == NODE_COMMAND)
		return (execute_command(&node->value.command, env));
	else if (node->type == NODE_PIPE)
		return (execute_pipe(node, env));
	else if (node->type == NODE_SUBSHELL)
		return (execute_subshell(node, env));
	else if (node->type == NODE_LOGICAL_AND)
	{
		left_status = execute_ast(node->left, env);
		if (left_status == 0)
			return (execute_ast(node->right, env));
		return (left_status);
	}
	else if (node->type == NODE_LOGICAL_OR)
	{
		left_status = execute_ast(node->left, env);
		if (left_status != 0)
			return (execute_ast(node->right, env));
		return (left_status);
	}
	else
	{
		ft_printf("Unknown node type\n");
		return (-1);
	}
}
