/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_array_manipulation.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:04:47 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/23 17:05:29 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>


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
