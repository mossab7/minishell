/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:22:59 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 17:05:49 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	put_fields_inplace(t_token_array *tokens, size_t index,
		t_token_array *fields, t_mask *mask)
{
	size_t	i;
	size_t	j;

	ft_memmove(tokens->items + index + fields->size, tokens->items + index,
		(tokens->size - index) * sizeof(*tokens->items));
	ft_memcpy(tokens->items + index, fields->items, fields->size
		* sizeof(*tokens->items));
	i = index;
	while ((i - index) < fields->size)
	{
		tokens->items[i].lexeme = vstr_construct(1, fields->items[i
				- index].lexeme->cstring);
		tokens->items[i].lexeme->mask->size = 0;
		j = 0;
		while (j < tokens->items[i].lexeme->size
			&& mask->cursor < mask->size)
		{
			mask_push_back(tokens->items[i].lexeme->mask,
				mask->items[mask->cursor]);
			mask->cursor++;
			j++;
		}
		i++;
	}
	tokens->size += fields->size;
}

void	tokens_field_split(t_token_array *tokens, size_t index)
{
	t_token_array	*fields;
	t_mask			*mask;
	t_token			*tk;

	mask = mask_construct();
	tk = &(tokens->items[index]);
	mask_copy_ignore_spaces(mask, tk->lexeme, 0);
	fields = tokenize_source((const char *)tk->lexeme->cstring);
	while (fields->size + tokens->size >= tokens->cap)
		tok_array_expand_anyhow(tokens);
	str_destruct(tk->lexeme);
	ft_memmove((tokens->items + index + 0), (tokens->items + index + 1),
		(tokens->size - index) * sizeof(*tokens->items));
	tokens->size--;
	if (fields->size)
		put_fields_inplace(tokens, index, fields, mask);
	toks_destroy(fields);
}
