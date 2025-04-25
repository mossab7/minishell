/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:23:24 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/20 18:24:10 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	set_pwd(char *pwd)
{
	t_context	*context;

	context = *get_context();
	if (context->pwd)
		ft_free(context->pwd);
	context->pwd = ft_strdup(pwd);
}

void	set_context_flag(t_ctx_flags flag)
{
	t_context	*context;

	context = *get_context();
	context->flags |= flag;
}

void	set_subshell_env(t_env *senv)
{
	t_context	*context;

	context = *get_context();
	set_context_flag(IN_SUBSHELL);
	context->subshell_env = senv;
}
