#include <zen.h>

void	tok_array_expand(t_token_array *vec)
{
	if(vec->size >= vec->cap)
	{
		vec->cap *= VEC_GROWTH_FAC;
		vec->items = ft_realloc(vec->items, vec->cap * sizeof(t_token),vec->size * sizeof(t_token));
	}
}

t_token_array	*tok_array_construct(void)
{
	t_token_array	*vec;

	vec = alloc(sizeof(*vec));
	vec->items = alloc(VEC_INIT_CAP * sizeof(t_token));
	vec->cap = VEC_INIT_CAP;
	vec->size = 0;
	return (vec);
}

t_token_array copy_tokens(t_token_array tokens)
{
    t_token_array tokens_copy;
    size_t i;

    i = 0;
    tokens_copy.size = tokens.size;
    tokens_copy.items = alloc(sizeof(t_token) * tokens.size);
    while (i < tokens.size)
    {
        tokens_copy.items[i].type = tokens.items[i].type;
        tokens_copy.items[i].lexeme = alloc(sizeof(t_lexer));
        tokens_copy.items[i].lexeme->cstring = ft_strdup(tokens.items[i].lexeme->cstring);
        i++;
    }
    return (tokens_copy);
}

void	tok_array_print(t_token_array *array)
{
	size_t	j;

	j = 0;
	while (j < array->size)
	{
		ft_printf("%u [ %s ]: |%s|\n", j, get_type_as_cstr(array->items[j].type), array->items[j].lexeme->cstring);
		j++;
	}
}
