/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:34:36 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 20:34:37 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

void	launch_command(t_command *cmd, t_env *env, char **args)
{
	t_string	*cmd_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (setup_redirections(cmd) == -1)
		exit(EXIT_FAILURE);
	cmd_path = search_path(env->path, args[0]);
	if (cmd_path)
	{
		env_join(env);
		execve(cmd_path->cstring, args, env->envp);
		str_destruct(cmd_path);
	}
	zen_elog("%s: command not found\n", args[0]);
	exit(127);
}

int	get_command_status(t_env *env, int status)
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

char	**collect_args(t_token_array *tokens)
{
	size_t	i;
	char	**args;

	i = 0;
	args = alloc(sizeof(char *) * (tokens->size + 1));
	while (i < tokens->size)
	{
		args[i] = ft_strdup(tokens->items[i].lexeme->cstring);
		i++;
	}
	return (args);
}

int	execute_command(t_command *cmd, t_env *env)
{
	int		status;
	char	**args;
	pid_t	current_pid;

	status = 0;
	if (cmd->args->size == 0)
		return (0);
	expand_command(get_context_env(), &cmd->args, 0);
	args = collect_args(cmd->args);
	env->last_command_status = execute_built_in_commands(cmd, args[0], env,
			args);
	if (env->last_command_status != -1)
		return (env->last_command_status);
	current_pid = fork();
	if (current_pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (current_pid == 0)
		launch_command(cmd, env, args);
	waitpid(current_pid, &status, 0);
	return (get_command_status(env, status));
}
