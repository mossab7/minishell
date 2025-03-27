/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:43:05 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 17:10:54 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	expand(t_env *env, t_token_array **tokens_array)
{
	size_t			iter;
	t_token_array	*tokens;

	tokens = *tokens_array;
	iter = 0;
	while (iter < tokens->size)
	{
		if (tokens->items[iter].type == TOK_WORD)
			tokens_expand(env, tokens, &iter, 0);
		else if (tokens->items[iter].type == TOK_WILD_CARD)
			wildcard_expand(tokens_array, &iter);
		iter++;
	}
}

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

static int	get_next_(t_token_array *tokens, size_t *cursor)
{
	find_next_expansion(tokens, cursor);
	if (*cursor < tokens->size)
		return (1);
	find_next_wcard(tokens, cursor);
	if (*cursor < tokens->size)
		return (1);
	return (0);
}

void	expand_command(t_env *env, t_token_array **tokens_array, size_t cursor)
{
	t_token_array	*tokens;
	int				is_export;

	is_export = 0;
	tokens = *tokens_array;
	if (ft_strcmp(tokens->items[cursor].lexeme->cstring, "export") == 0)
		is_export = 1;
	while (true)
	{
		tokens = *tokens_array;
		if (!get_next_(tokens, &cursor))
			break ;
		if (tokens->items[cursor].type == TOK_WORD)
			tokens_expand(env, tokens, &cursor, is_export);
		else if (tokens->items[cursor].type == TOK_WILD_CARD)
			wildcard_expand(tokens_array, &cursor);
	}
}
