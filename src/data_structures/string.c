/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:40:18 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/21 17:45:37 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static void	str_expand_anyhow(t_string *vec)
{
	vec->cap *= VEC_GROWTH_FAC;
	vec->cstring = ft_realloc(vec->cstring, vec->cap, vec->size);
	mask_expand_anyhow(vec->mask);
}

void	str_expand(t_string *vec)
{
	if(vec->size + 1 >= vec->cap)
		str_expand_anyhow(vec);
}

void str_push_back(t_string *vec, uintptr_t item)
{
	str_expand(vec);
	vec->cstring[vec->size++] = item;
	vec->cstring[vec->size] = 0;
}

t_string	*str_construct()
{
	t_string	*vec;

	vec = alloc(sizeof(*vec));
	vec->cstring = alloc(VEC_INIT_CAP);
	vec->cap = VEC_INIT_CAP;
	vec->mask = mask_construct();
	vec->size = 0;
	vec->cursor = 0;
	return (vec);
}

void	str_join(t_string	*vec, size_t count, ...)
{
	size_t		i;
	va_list args;

	va_start(args, count);
	i = 0;
	while(i < count)
	{
		str_append(va_arg(args, char *), vec);
		i++;
	}
	va_end(args);
}

t_string	*vstr_construct(size_t count, ...)
{
	t_string	*vec;
	size_t		i;
	va_list args;

	va_start(args, count);
	vec = str_construct();
	i = 0;
	while(i < count)
	{
		str_append(va_arg(args, char *),vec);
		i++;
	}
	va_end(args);
	return (vec);
}

void	str_destruct(t_string *vec)
{
	if (vec)
	{
		if (vec->cstring)
			ft_free(vec->cstring);
		vec->cstring = NULL;
		if (vec->mask)
			mask_destroy(vec->mask);
		vec->mask = NULL;
		ft_free(vec);
	}
}

void	str_append(char *src, t_string *vec)
{
	size_t	index;
	if(!src)
		return;
	index = 0;
	while (src[index])
	{
		str_push_back(vec, src[index]);
		mask_push_back(vec->mask, 0);
		index++;
	}
}

void	str_overwrite(char *src, t_string *vec)
{
	if(!src)
		return;
	vec->size = 0;
	vec->cstring[vec->size] = 0;
	if (!*src)
		return ;
	str_append(src, vec);
}

int	str_search_using_cursor(t_string *vec, const char *text)
{
	char	*cursor;

	cursor = ft_strnstr(((const char *) vec->cstring + vec->cursor), text, vec->size);
	return ((int)(cursor - vec->cstring));
}

int	str_search(t_string *vec, const char *text)
{
	char	*cursor;

	cursor = ft_strnstr(((const char *) vec->cstring), text, vec->size);
	return ((int)(cursor - vec->cstring));
}

void	str_print(t_string *str)
{
	ft_printf("Mask size: %u\n", str->mask->size);
	ft_printf("str  size: %u\n", str->size);
	ft_printf("[%u]# %s\n", str->size, str->cstring);
	ft_printf("     ");
	for (size_t i = 0; i < str->mask->size; i++)
		ft_printf("%u", str->mask->items[i]);
	ft_printf("\n");
}

void	str_shift_left(t_string *str, int new_dest, int size)
{
	ft_memmove((str->cstring + new_dest), (str->cstring + new_dest + size), 
	    (str->size - new_dest - size));
	ft_memmove((str->mask->items + new_dest), (str->mask->items + new_dest + size), 
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
	int	cursor;
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
		for (size_t index = cursor; index < (cursor + repl_size); index++)
			string->mask->items[index] = EXPANDED;
		string->cstring[string->size] = 0;
		cursor = str_search(string, (const char *)which->cstring);
	}
}
