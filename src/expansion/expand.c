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

void	expand(t_env *env, t_token_array **tokens_array)
{
	size_t			iter;
	t_token_array	*tokens = *tokens_array;
	iter = 0;
	while (iter < tokens->size)
	{
		if (tokens->items[iter].type == TOK_WORD)
			string_expand(env, tokens, &iter);
		else if(tokens->items[iter].type == TOK_WILD_CARD)
			wildcard_expand(tokens_array, &iter);
		iter++;
	}
}
