#include <zen.h>

void	tok_array_expand(t_token_array *vec)
{
	if (!vec->size)
		return ;
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
	vec->items = alloc(1 * sizeof(t_token));
	vec->input = NULL;
	vec->cap = 1;
	vec->size = 0;
	vec->here_doc_active = false;
	vec->syntax_error = false;
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
        tokens_copy.items[i].lexeme = alloc(sizeof(t_string)); // mask
        tokens_copy.items[i].lexeme->cstring = ft_strdup(tokens.items[i].lexeme->cstring);
		tokens_copy.items[i].mask = mask_construct();
        i++;
    }
    return (tokens_copy);
}

void	tok_array_print(t_token_array *array)
{
	size_t	j;
	t_token	*tok;

	j = 0;
	while (j < array->size)
	{
		tok = &array->items[j];
		if (tok->type == TOK_EOF)
			break ;
		ft_printf("# %s\n", tok->lexeme->cstring);
		ft_printf("  ");
		for (size_t i = 0; i < tok->mask->size; i++)
			ft_printf("%u", tok->mask->items[i]);
		ft_printf("\n");
		j++;
	}
}
