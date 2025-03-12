#include <zen.h>

static void	str_expand__(t_string *vec)
{
	vec->cap *= VEC_GROWTH_FAC;
	vec->cstring = ft_realloc(vec->cstring, vec->cap, vec->size);
}

void	str_expand(t_string *vec)
{
	if(vec->size + 1 >= vec->cap)
		str_expand__(vec);
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
// TODO: @opt_msk: an optional mask to edit alongside with the actual string
// Mask:    11111111
// Content: $x jjjjj
void	str_substitute(t_string *vec, char *repl, char *which)
{
	// TODO: A function that takes a specific string and puts it instead of some other string in the whole string
	// Example: $x $x, look for the value of x and do str_substitute(v, value_x, "$x");
	char	*cursor;
	int		repl_size;
	int		which_size;

	repl_size = ft_strlen(repl);
	which_size = ft_strlen(which);
	cursor = ft_strnstr(((const char *) vec->cstring), (const char *) which, vec->size);
	while (cursor)
	{
		while ((int)vec->size - which_size + repl_size >= (int)vec->cap)
			str_expand__(vec);
		// Before: "hello xyz"
		// After: " xyz"
		ft_memmove(cursor, cursor + which_size, ft_strlen(cursor + which_size));

		// ft_memmove(&opt_msk->item[cursor - vec->cstring], 
		// 	 &opt_msk->item[cursor - vec->cstring] + which_size,
		// 	 ft_strlen(cursor + which_size));
		vec->size -= which_size;

		// Before: " xyz"
		// After: "0000 xyz"
		ft_memmove(cursor + repl_size, cursor, ft_strlen(cursor + which_size));
		vec->size += repl_size;
		// Before: " xyz"
		// After: "repl xyz"
		ft_memcpy(cursor, (const char *)repl, repl_size);
		vec->cstring[vec->size] = 0;
		cursor = ft_strnstr(((const char *) vec->cstring), (const char *) which, vec->size);
	}
}
