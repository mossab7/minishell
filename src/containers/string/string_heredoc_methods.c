/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_heredoc_methods.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:08:55 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/27 18:03:16 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static int	should_dollar_sign_bremoved(t_string *string)
{
	if ((size_t)(string->cursor + 1) == string->size)
		return ((string->mask->context & DOUBLE_QUOTED)
			|| (string->mask->context & SINGLE_QUOTED));
	return (string->mask->items[string->cursor + 1] & SINGLE_QUOTED
		|| string->mask->items[string->cursor + 1] & DOUBLE_QUOTED);
}

void	jump_dollar_signs(t_string *string)
{
	char	c;
	t_u8	ctx;

	string->cursor = 0;
	while ((size_t) string->cursor < string->size)
	{
		c = string->cstring[string->cursor];
		ctx = string->mask->items[string->cursor];
		if (c == '$' && ctx & NOT_QUOTED)
		{
			if (should_dollar_sign_bremoved(string))
				str_shift_left(string, string->cursor, 1);
			else if (string->cstring[string->cursor + 1] == c)
				string->cursor++;
		}
		string->cursor++;
	}
	string->cstring[string->size] = 0;
	string->cursor = 0;
}

int	delimiter_reached(t_string *delim, t_string *line)
{
	if (ft_strcmp(line->cstring, delim->cstring) == 0)
	{
		str_destruct(line);
		return (1);
	}
	expand_if_delim_not_quoted(delim, line);
	return (0);
}
