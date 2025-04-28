/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_subs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:53:07 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/28 15:09:08 by lazmoud          ###   ########.fr       */
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
	ft_memmove((str->mask->ids + new_dest),
		(str->mask->ids + new_dest + size),
		sizeof(*(str->mask->ids)) * (str->mask->size - new_dest - size));
	str->size -= size;
	str->mask->size -= size;
	str->cursor -= size;
}

void	str_shift_right(t_string *string, int old_dest, int size)
{
	if (!size)
		return ;
	ft_memmove((string->cstring + old_dest + size),
		(string->cstring + old_dest),
		(string->size - old_dest));
	ft_memmove((string->mask->items + old_dest + size),
		(string->mask->items + old_dest),
		(string->mask->size - old_dest));
	ft_memmove((string->mask->ids + old_dest + size),
		(string->mask->ids + old_dest),
		sizeof(*(string->mask->ids)) * (string->mask->size - old_dest));
	string->size += size;
	string->mask->size += size;
	string->cursor += size;
}

static void	reserve_space(t_string *string, int cursor,
							size_t which_size, size_t repl_size)
{
	while (string->size + repl_size >= string->cap)
		str_expand_anyhow(string);
	str_shift_left(string, cursor, which_size);
	str_shift_right(string, cursor, repl_size);
}

void	str_substitute(t_string *string, char *repl, t_string *which)
{
	int				cursor;
	unsigned int	id;
	t_u8			ctx_mask;
	size_t			repl_size;

	cursor = str_search(string, (const char *)which->cstring, 0);
	repl_size = ft_strlen(repl);
	if (cursor >= 0)
	{
		while (string->mask->items[cursor] & SINGLE_QUOTED)
		{
			cursor = str_search(string,
					(const char *)which->cstring, (cursor + 1));
			if (cursor < 0)
				return ;
		}
		ctx_mask = string->mask->items[cursor];
		id = string->mask->ids[cursor];
		reserve_space(string, cursor, which->size, repl_size);
		ft_memcpy((string->cstring + cursor), (const char *)repl, repl_size);
		ft_memset(string->mask->items + cursor,
			(ctx_mask | EXPANDED), repl_size);
		ft_memset(string->mask->ids + cursor, id, repl_size);
		string->cstring[string->size] = 0;
	}
}
