/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_next_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:45:59 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/05 19:01:13 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	is_dollar_sign_not_expandable(t_string *string, int index)
{
	return (ft_isspace(string->cstring[index + 1])
		|| string->mask->items[index] == SINGLE_QUOTED
		|| string->mask->items[index] == EXPANDED
		|| !string->cstring[index + 1]);
}

static int	look_for_valid(t_string *string)
{
	int	index;
	int	prev_cursor;

	prev_cursor = string->cursor;
	string->cursor = 0;
	index = -1;
	while (string->size)
	{
		if ((size_t)string->cursor >= string->size)
		{
			index = 0;
			break ;
		}
		index = str_search_using_cursor(string, "$");
		if (index < 0 || index == (int)string->size - 1)
			return (0);
		if (is_dollar_sign_not_expandable(string, index))
		{
			string->cursor = (index + 1);
			continue ;
		}
		string->cursor = prev_cursor;
		return (index >= 0);
	}
	return (index != -1);
}

void	find_next_expansion(t_token_array *tokens, size_t *cursor)
{
	*cursor = 0;
	while (*cursor < tokens->size)
	{
		if (look_for_valid(tokens->items[*cursor].lexeme))
			break ;
		(*cursor)++;
	}
	while (*cursor < tokens->size
		&& (tokens->items[*cursor].type != TOK_WORD))
		(*cursor)++;
}
