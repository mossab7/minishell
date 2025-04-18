/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:19:43 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/23 17:24:39 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

t_context	**get_context(void)
{
	static t_context	*context = NULL;

	if (context == NULL)
	{
		context = alloc(sizeof(t_context));
		context->flags = 0;
		set_context_flag(FLAG_READLINE_ACTIVE);
		context->input = NULL;
		context->env = NULL;
		context->pwd = getcwd(NULL, 0);
	}
	return (&context);
}

void	init_context(t_string *initial_input, t_env *env)
{
	t_context	*context;

	context = *get_context();
	context->flags = 0;
	context->input = initial_input;
	if (!context->env)
		context->env = env;
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
	return (context->env);
}

void	set_pwd(char *pwd)
{
	t_context	*context;

	context = *get_context();
	context->pwd = pwd;
}
