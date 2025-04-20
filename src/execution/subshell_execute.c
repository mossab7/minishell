/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:27:50 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/20 18:30:29 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	subshell_execute(t_ast *node, t_env *env)
{
	t_env	*subshell_env;
	int		result;

	subshell_env = env_copy(env);
	set_subshell_env(subshell_env);
	if (setup_redirections(&node->u_value.command) == -1)
	{
		env_destroy(subshell_env);
		safe_exit(EXIT_FAILURE);
	}
	result = execute_ast(node->left, subshell_env);
	env_destroy(subshell_env);
	unset_subshell_env();
	safe_exit(result);
}
