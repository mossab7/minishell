/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_getters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:22:46 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/20 18:26:13 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

t_context	**get_context(void)
{
	static t_context	*context = NULL;
	char				internal_buff[PATH_MAX];

	if (context == NULL)
	{
		context = alloc(sizeof(t_context));
		context->flags = 0;
		set_context_flag(FLAG_READLINE_ACTIVE);
		context->input = NULL;
		context->env = NULL;
		context->subshell_env = NULL;
		context->pwd = getcwd(internal_buff, PATH_MAX);
	}
	return (&context);
}

t_string	*get_context_input(void)
{
	t_context	*context;

	context = *get_context();
	return (context->input);
}

t_env	*get_context_env(void)
{
	t_context	*context;

	context = *get_context();
	if (get_is_insubshell())
		return (context->subshell_env);
	return (context->env);
}

int	get_is_insubshell(void)
{
	t_context	*context;

	context = *get_context();
	return (context->flags & IN_SUBSHELL);
}
