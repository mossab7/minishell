/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:04:39 by mbouhia           #+#    #+#             */
/*   Updated: 2025/04/20 18:25:51 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

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

void	unset_subshell_env(void)
{
	t_context	*context;

	context = *get_context();
	unset_context_flag(IN_SUBSHELL);
	context->subshell_env = NULL;
}
