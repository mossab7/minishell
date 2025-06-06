/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:19:43 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/28 14:26:43 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	init_context(t_string *initial_input, t_env *env)
{
	t_context	*context;

	context = *get_context();
	context->flags = 0;
	context->input = initial_input;
	context->_id_quoting = 1;
	if (!context->env)
		context->env = env;
}
