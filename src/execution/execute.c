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
		if (setup_redirections(&node->u_value.command) == -1)
		{
			env_destroy(subshell_env);
			exit(EXIT_FAILURE);
		}
		result = execute_ast(node->left, subshell_env);
		env_destroy(subshell_env);
		exit(result);
	}
	waitpid(pid, &status, 0);
	return (env->last_command_status = WEXITSTATUS(status));
}

int	execute_logical_or(t_ast *node, t_env *env, int left_status)
{
	left_status = execute_ast(node->left, env);
	if (left_status != 0)
		return (execute_ast(node->right, env));
	return (left_status);
}

int	execute_logical_and(t_ast *node, t_env *env, int left_status)
{
	left_status = execute_ast(node->left, env);
	if (left_status == 0)
		return (execute_ast(node->right, env));
	return (left_status);
}

int	execute_ast(t_ast *node, t_env *env)
{
	int	left_status;

	if (!node)
		return (0);
	left_status = 0;
	if (node->type == NODE_COMMAND)
		return (execute_command(&node->u_value.command, env));
	else if (node->type == NODE_PIPE)
		return (execute_pipe(node, env));
	else if (node->type == NODE_SUBSHELL)
		return (execute_subshell(node, env));
	else if (node->type == NODE_LOGICAL_AND)
		return (execute_logical_and(node, env, left_status));
	else if (node->type == NODE_LOGICAL_OR)
		return (execute_logical_or(node, env, left_status));
	else
	{
		ft_printf("Unknown node type\n");
		return (-1);
	}
}
