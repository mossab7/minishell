/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:43:05 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/14 22:09:53 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static void	find_next_wcard(t_token_array *tokens, size_t *cursor)
{
	*cursor = 0;
	while (*cursor < tokens->size
		&& !ft_strchr(tokens->items[*cursor].lexeme->cstring, '*'))
		(*cursor)++;
	while (*cursor < tokens->size
		&& (tokens->items[*cursor].type != TOK_WILD_CARD))
		(*cursor)++;
}

void	pathname_expansion(t_token_array **tokens_array)
{
	t_token_array	*tokens;
	size_t			cursor;

	while (true)
	{
		tokens = *tokens_array;
		find_next_wcard(tokens, &cursor);
		if (cursor >= tokens->size)
			break ;
		wildcard_expand(tokens_array, &cursor);
	}
}

void	expand_command(t_env *env, t_token_array **tokens_array)
{
	t_token_array	*tokens;
	int				is_export;
	size_t			cursor;

	is_export = 0;
	cursor = 0;
	tokens = *tokens_array;
	if (ft_strcmp(tokens->items[cursor].lexeme->cstring, "export") == 0)
		is_export = 1;
	while (true)
	{
		find_next_expansion(tokens, &cursor);
		if (cursor >= tokens->size)
			break ;
		if (tokens->items[cursor].type == TOK_WORD)
		{
			tokens->items[cursor].lexeme->cursor = 0;
			tokens_expand(env, tokens, &cursor);
		}
	}
	field_split(tokens_array, is_export);
	pathname_expansion(tokens_array);
}
