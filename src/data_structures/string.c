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
	if(vec)
	{
		ft_free(vec->cstring);
		if (vec->mask)
			mask_destroy(vec->mask);
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

int	str_search(t_string *vec, const char *text)
{
	char	*cursor;

	cursor = ft_strnstr(((const char *) vec->cstring), text, vec->size);
	return ((int)(cursor - vec->cstring));
}

void	str_print(t_string *str)
{
	ft_printf("# %s\n", str->cstring);
	ft_printf("  ");
	for (size_t i = 0; i < str->mask->size; i++)
		ft_printf("%u", str->mask->items[i]);
	ft_printf("\n");
}

void	str_substitute(t_string *vec, char *repl, char *which)
{
	u8	stat;
	int	cursor;
	int	repl_size;
	int	which_size;

	repl_size = ft_strlen(repl);
	which_size = ft_strlen(which);
	cursor = str_search(vec, (const char *)which);
	while (cursor >= 0)
	{
		while ((int)vec->size - which_size + repl_size >= (int)vec->cap)
			str_expand_anyhow(vec);
		stat = (vec->mask->items[cursor]);
		{
			ft_memmove((vec->cstring + cursor), (vec->cstring + cursor + which_size), 
				 (vec->size - cursor));
			ft_memmove((vec->mask->items + cursor), (vec->mask->items + cursor + which_size), 
				 (vec->mask->size - cursor));
			vec->size -= which_size;
			vec->mask->size -= which_size;
		}

		{

			ft_memmove((vec->cstring + cursor + repl_size),
				 (vec->cstring + cursor), 
				 (vec->size - cursor));
			ft_memmove((vec->mask->items + cursor + repl_size),
				 (vec->mask->items + cursor),
				 (vec->mask->size - cursor));
			vec->size += repl_size;
			vec->mask->size += repl_size;
		}

		// huehf value wiijwdi
		// huehf       wiijwdi
		//       ^
		ft_memcpy((vec->cstring + cursor), (const char *)repl, repl_size);
		for (int index = 0; index < repl_size; index++)
			vec->mask->items[(index + cursor)] = stat;
		vec->cstring[vec->size] = 0;
		cursor = str_search(vec, (const char *)which);
	}
}
