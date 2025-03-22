/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:40:42 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/22 20:28:19 by lazmoud          ###   ########.fr       */
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

t_token_array	*tokens_copy(t_token_array *other)
{
	t_token_array	*copy;
	size_t			i;
	size_t			j;

	copy =  alloc(sizeof(*copy));
	ft_memcpy(copy, other, sizeof(*copy));
	copy->items = alloc(copy->cap * sizeof(t_token));
	i = 0;
	while (i < other->size)
	{
		copy->items[i].lexeme = vstr_construct(1, other->items[i].lexeme->cstring);
		copy->items[i].type = other->items[i].type;
		j = 0;
		while (j < other->items[i].lexeme->mask->size)
			mask_push_back(copy->items[i].lexeme->mask, other->items[i].lexeme->mask->items[j++]);
		i++;
	}
	return (copy);
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
	size_t	i;

	i = 0;
	if (vec)
	{
		if (vec->items)
		{
			while (i < vec->size)
			{
				str_destruct(vec->items[i].lexeme);
				vec->items[i++].lexeme = NULL;
			}
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
		"TOK_WILD_CARD",
		"NUM",
		"OPAREN",
		"CPAREN",
		"EOF",
	};
	return (them[type]);
}
