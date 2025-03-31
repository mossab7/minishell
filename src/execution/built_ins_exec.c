/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:34:24 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 21:25:55 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

void	reset_redirections(int *original_stdin, int *original_stdout)
{
	dup2(*original_stdin, STDIN_FILENO);
	dup2(*original_stdout, STDOUT_FILENO);
	close(*original_stdin);
	close(*original_stdout);
	*original_stdin = 0;
	*original_stdout = 0;
}

int	setup_builtin_redirections(t_command *cmd, t_type type)
{
	static int	original_stdin;
	static int	original_stdout;
	int			redirection_result;

	if (type == setup)
	{
		original_stdin = dup(STDIN_FILENO);
		original_stdout = dup(STDOUT_FILENO);
		if (original_stdin == -1 || original_stdout == -1)
		{
			original_stdin = 0;
			original_stdout = 0;
			return ((perror("dup")), -1);
		}
		redirection_result = setup_redirections(cmd);
		if (redirection_result == -1)
			return (reset_redirections(&original_stdin, &original_stdout), -1);
	}
	else
		reset_redirections(&original_stdin, &original_stdout);
	return (0);
}

void	init_builtin_commands(t_built_in_command *functions,
		char **function_names)
{
	functions[0] = built_in_cd;
	functions[1] = built_in_echo;
	functions[2] = built_in_env;
	functions[3] = built_in_exit;
	functions[4] = built_in_export;
	functions[5] = built_in_unset;
	functions[6] = NULL;
	function_names[0] = "cd";
	function_names[1] = "echo";
	function_names[2] = "env";
	function_names[3] = "exit";
	function_names[4] = "export";
	function_names[5] = "unset";
	function_names[6] = NULL;
}

int	execute_built_in_commands(t_command *cmd, char *command, t_env *env,
		char **args)
{
	t_built_in_command	functions[BUILT_IN_COMMANDS_COUNT];
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
	command_result = functions[i](env, cmd->argc, args);
	setup_builtin_redirections(cmd, teardown);
	return (command_result);
}
