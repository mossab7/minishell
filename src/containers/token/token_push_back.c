/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_push_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:01:26 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/14 22:08:27 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	token_push_back(t_token	*tok, char c, t_u8 state)
{
	str_push_back(tok->lexeme, c);
	mask_push_back(tok->lexeme->mask, state);
}

void	tok_array_push_back(t_token_array *array, t_token *tok)
{
	size_t	cur;

	tok_array_expand(array);
	array->items[array->size].type = tok->type;
	array->items[array->size].lexeme = str_construct();
	cur = 0;
	while (cur < tok->lexeme->size)
	{
		token_push_back(&array->items[array->size],
			tok->lexeme->cstring[cur], tok->lexeme->mask->items[cur]);
		cur++;
	}
	array->size++;
}
