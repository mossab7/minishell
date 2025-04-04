/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_subs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:53:07 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 15:48:31 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	str_shift_left(t_string *str, int new_dest, int size)
{
	ft_memmove((str->cstring + new_dest), (str->cstring + new_dest + size),
		(str->size - new_dest - size));
	ft_memmove((str->mask->items + new_dest),
		(str->mask->items + new_dest + size),
		(str->mask->size - new_dest - size));
	str->size -= size;
	str->mask->size -= size;
	str->cursor -= size;
}

void	str_shift_right(t_string *string, int old_dest, int size)
{
	ft_memmove((string->cstring + old_dest + size),
		(string->cstring + old_dest),
		(string->size - old_dest));
	ft_memmove((string->mask->items + old_dest + size),
		(string->mask->items + old_dest),
		(string->mask->size - old_dest));
	string->size += size;
	string->mask->size += size;
	string->cursor += size;
}

void	str_substitute(t_string *string, char *repl, t_string *which)
{
	int		cursor;
	size_t	repl_size;

	cursor = str_search(string, (const char *)which->cstring);
	repl_size = ft_strlen(repl);
	while (cursor >= 0)
	{
		while (string->size - which->size + repl_size >= string->cap)
			str_expand_anyhow(string);
		str_shift_left(string, cursor, which->size);
		str_shift_right(string, cursor, repl_size);
		ft_memcpy((string->cstring + cursor), (const char *)repl, repl_size);
		ft_memset((string->mask->items + cursor), EXPANDED, repl_size);
		string->cstring[string->size] = 0;
		cursor = str_search(string, (const char *)which->cstring);
	}
}
