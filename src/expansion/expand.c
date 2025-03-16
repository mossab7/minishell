/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:43:05 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 06:43:08 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	expand_value(t_token *tok, t_env *env, t_token_array *tokens)
{
	size_t		iter;

	iter = (tok - tokens->items);
	string_expand(env, tok, tokens);
	if (!tok->lexeme->size)
	{
		ft_memmove(tok, (tok + 1), (tokens->size - iter) * sizeof(*tokens->items));
		tokens->size--;
		return ;
	}
}

void	expand(t_env *env, t_token_array *tokens)
{
	t_token			*tok;
	size_t			iter;

	iter = 0;
	while (iter < tokens->size)
	{
		tok = &tokens->items[iter];
		if (tok->type == TOK_WORD)
			expand_value(tok, env, tokens);
		iter++;
	}
}
