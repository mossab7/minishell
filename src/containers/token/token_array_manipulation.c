/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_array_manipulation.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:04:47 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 16:04:29 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

t_token_array	*tokens_copy(t_token_array *other)
{
	t_token_array	*copy;
	size_t			i;
	size_t			j;

	copy = alloc(sizeof(*copy));
	ft_memcpy(copy, other, sizeof(*copy));
	copy->items = alloc(copy->cap * sizeof(t_token));
	i = 0;
	while (i < other->size)
	{
		copy->items[i].lexeme = vstr_construct(1,
				other->items[i].lexeme->cstring);
		copy->items[i].type = other->items[i].type;
		j = 0;
		while (j < other->items[i].lexeme->mask->size)
			mask_push_back(copy->items[i].lexeme->mask,
				other->items[i].lexeme->mask->items[j++]);
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

char	*get_type_as_cstr(t_token_type type)
{
	char	*them[TOK_SIZE];

	them[TOK_NONE] = "TOK_NONE";
	them[TOK_EQ] = "TOK_EQ";
	them[TOK_PEQ] = "TOK_PEQ";
	them[TOK_PIPE] = "PIPE";
	them[TOK_SYMBOL] = "SYMBOL";
	them[TOK_AND] = "AND";
	them[TOK_INPUT_REDIRECT] = "INPUT_REDIRECT";
	them[TOK_OUTPUT_REDIRECT] = "OUTPUT_REDIRECT";
	them[TOK_APPEND] = "APPEND";
	them[TOK_HEREDOC] = "HEREDOC";
	them[TOK_EXPANSION_MARK] = "EXPANSION_MARK";
	them[TOK_LOGICAL_OR] = "LOGICAL_OR";
	them[TOK_LOGICAL_AND] = "LOGICAL_AND";
	them[TOK_NEWLINE] = "NEWLINE";
	them[TOK_ESCAPE] = "ESCAPE";
	them[TOK_COMMENT] = "COMMENT";
	them[TOK_WORD] = "WORD";
	them[TOK_WILD_CARD] = "TOK_WILD_CARD";
	them[TOK_NUM] = "NUM";
	them[TOK_OPAREN] = "OPAREN";
	them[TOK_CPAREN] = "CPAREN";
	them[TOK_EOF] = "EOF";
	return (them[type]);
}

t_token	*ft_tokdup(t_token token)
{
	t_token	*new_token;
	size_t	index;

	new_token = alloc(sizeof(t_token));
	new_token->type = token.type;
	new_token->lexeme = vstr_construct(1, token.lexeme->cstring);
	new_token->lexeme->mask = mask_construct();
	index = 0;
	while (index < token.lexeme->mask->size)
	{
		mask_push_back(new_token->lexeme->mask,
			token.lexeme->mask->items[index]);
		index++;
	}
	return (new_token);
}
