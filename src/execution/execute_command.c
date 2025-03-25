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

int	execute_command(t_command *cmd, t_env *env)
{
	int		status;
	pid_t	current_pid;

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
		launch_command(cmd, env);
	waitpid(current_pid, &status, 0);
	return (get_command_status(env, status));
}
