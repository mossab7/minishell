/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_copying.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:18:17 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 17:19:00 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	copy_left_tokens(t_token_array *tokens, t_token_array *new_tokens_array,
		size_t cursor)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < cursor)
	{
		new_tokens_array->items[i].lexeme = vstr_construct(1,
				tokens->items[i].lexeme->cstring);
		new_tokens_array->items[i].type = tokens->items[i].type;
		if (new_tokens_array->items[i].lexeme->mask == NULL)
			new_tokens_array->items[i].lexeme->mask = mask_construct();
		j = 0;
		while (j < tokens->items[i].lexeme->mask->size)
		{
			mask_push_back(new_tokens_array->items[i].lexeme->mask,
				tokens->items[i].lexeme->mask->items[j]);
			j++;
		}
		i++;
	}
}

void	copy_entries(t_string_vector *entries, t_token_array *new_tokens_array,
		size_t cursor)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < entries->size)
	{
		new_tokens_array->items[cursor + i].lexeme = vstr_construct(1,
				entries->cstrings[i]);
		if (new_tokens_array->items[cursor + i].lexeme->mask == NULL)
			new_tokens_array->items[cursor + i].lexeme->mask = mask_construct();
		new_tokens_array->items[cursor + i].type = TOK_WORD;
		j = 0;
		while (j < ft_strlen(entries->cstrings[i]))
		{
			mask_push_back(new_tokens_array->items[cursor + i].lexeme->mask,
				NOT_QUOTED);
			j++;
		}
		i++;
	}
}

void	copy_right_tokens(t_token_array *tokens,
		t_token_array *new_tokens_array, size_t cursor, size_t entries_size)
{
	size_t	i;
	size_t	j;
	size_t	dest_index;

	i = cursor + 1;
	dest_index = cursor + entries_size;
	while (i < tokens->size && dest_index < new_tokens_array->size)
	{
		new_tokens_array->items[dest_index].lexeme = vstr_construct(1,
				tokens->items[i].lexeme->cstring);
		if (new_tokens_array->items[dest_index].lexeme->mask == NULL)
			new_tokens_array->items[dest_index].lexeme->mask = mask_construct();
		new_tokens_array->items[dest_index].type = tokens->items[i].type;
		j = 0;
		while (j < tokens->items[i].lexeme->mask->size)
		{
			mask_push_back(new_tokens_array->items[dest_index].lexeme->mask,
				tokens->items[i].lexeme->mask->items[j]);
			j++;
		}
		i++;
		dest_index++;
	}
}
