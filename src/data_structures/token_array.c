/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:40:42 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/17 16:10:19 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	tok_array_expand_anyhow(t_token_array *vec)
{
	vec->cap *= VEC_GROWTH_FAC;
	vec->items = ft_realloc(vec->items, vec->cap * sizeof(t_token),vec->size * sizeof(t_token));
	for (size_t i = vec->size; i < vec->cap; ++i)
		vec->items[i].lexeme = str_construct();
}

void	tok_array_expand(t_token_array *vec)
{
	if(vec->size >= vec->cap)
		tok_array_expand_anyhow(vec);
}

t_token_array	*tok_array_construct(void)
{
	t_token_array	*vec;

	vec = alloc(sizeof(*vec));
	vec->items = alloc(VEC_INIT_CAP * sizeof(t_token));
	vec->cap = VEC_INIT_CAP;
	vec->size = 0;
	vec->here_doc_active = false;
	vec->syntax_error = false;
	for (size_t i = vec->size; i < vec->cap; ++i)
	{
		vec->items[i].lexeme = str_construct();
		vec->items[i].type = TOK_NONE;
	}
	vec->current = (vec->items + vec->size);
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
        tokens_copy.items[i].lexeme = vstr_construct(1, tokens.items[i].lexeme->cstring);
        i++;
    }
    return (tokens_copy);
}

void	tok_array_print(t_token_array *array)
{
	size_t	j;
	t_token	*tok;

	j = 0;
	printf("============================size : %zu ===================================\n", array->size);
	while (j < array->size)
	{
		tok = &array->items[j];
		if (tok->type == TOK_EOF)
			break ;
		printf("tok->type = %s\n", get_type_as_cstr(tok->type));
		str_print(tok->lexeme);
		j++;
	}
	printf("==========================================================================\n");
}

void	toks_destroy(t_token_array	*vec)
{
	if (vec)
	{
		if (vec->items)
		{
			for (size_t i = 0; i < vec->cap; ++i)
				str_destruct(vec->items[i].lexeme);
			ft_free(vec->items);
		}
		ft_free(vec);
	}
}

void	token_next(t_token_array *vec)
{
	vec->size++;
	tok_array_expand(vec);
	vec->current = (vec->items + vec->size);
}

char *get_type_as_cstr(t_token_type type)
{
	char *them[TOK_SIZE] = {
		"TOK_NONE",
		"TOK_EQ",
		"TOK_PEQ",
		"PIPE",
		"SYMBOL",
		"AND",
		"INPUT_REDIRECT",
		"OUTPUT_REDIRECT",
		"APPEND",
		"HEREDOC",
		"EXPANSION_MARK",
		"LOGICAL_OR",
		"LOGICAL_AND",
		"NEWLINE",
		"ESCAPE",
		"COMMENT",
		"WORD",
		"NUM",
		"OPAREN",
		"CPAREN",
		"EOF",
	};
	return (them[type]);
}
