/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:42:01 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/20 18:30:00 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

bool	is_ambiguous_redirect(t_string *filename)
{
	struct stat	file_stat;

	if (!filename->cstring)
	{
		zen_elog("%s: ambiguous redirect\n", filename->cstring);
		return (true);
	}
	if (stat(filename->cstring, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			zen_elog("%s/: Is a directory\n", filename->cstring);
			return (true);
		}
	}
	// TODO: the filename->cstring we get here should be not expanded
	if (ft_strchr((char *)filename->cstring, '$')
		|| ft_strchr((char *)filename->cstring, '*')
		|| ft_strchr((char *)filename->cstring, '?'))
	{
		zen_elog("%s: ambiguous redirect\n", filename->cstring);
		return (true);
	}
	return (false);
}

int	execute_subshell(t_ast *node, t_env *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		subshell_execute(node, env);
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
