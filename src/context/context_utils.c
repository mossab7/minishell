/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:04:39 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/24 18:04:40 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

void	set_context_flag(t_ctx_flags flag)
{
	t_context	*context;

	context = *get_context();
	context->flags |= flag;
}

void	unset_context_flag(t_ctx_flags flag)
{
	t_context	*context;

	context = *get_context();
	context->flags &= ~flag;
}

void	toggle_context_flag(t_ctx_flags flag)
{
	t_context	*context;

	context = *get_context();
	context->flags ^= flag;
}

bool	check_context_flag(t_ctx_flags flag)
{
	t_context	*context;

	context = *get_context();
	return (context->flags & flag);
}
