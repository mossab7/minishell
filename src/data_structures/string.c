#include <zen.h>

void	str_expand(t_string *vec)
{
	if(vec->size + 1 >= vec->cap)
	{
		vec->cap *= VEC_GROWTH_FAC;
		vec->cstring = ft_realloc(vec->cstring, vec->cap,vec->size);
	}
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

void	str_append(char *src,t_string *vec)
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
