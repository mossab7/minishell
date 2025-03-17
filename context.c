/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:19:43 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/17 16:25:34 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

t_context **get_context()
{
    static t_context *context = NULL;

    if (context == NULL)
    {
        context = alloc(sizeof(t_context));
        context->siginit_received = false;
        context->readline_active = 1;
		context->env = NULL;
    }
    return &context;
}

void	init_context(t_string *initial_input, t_env *env)
{
	t_context	*context;

	context = *get_context();
	context->readline_active = 0;
	context->siginit_received = false;
	context->input = initial_input;
	if (!context->env)
		context->env = env;
}

t_string	*get_context_input()
{
	t_context	*context;

	context = *get_context();
	return (context->input);
}

t_env	*get_context_env()
{
	t_context	*context;

	context = *get_context();
	return (context->env);
}
