/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_wild_card_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:04:51 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 18:05:22 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>


bool	should_skip_expansion(t_token_array *tokens, size_t cursor)
{
	return (cursor >= tokens->size);
}

void	handle_no_matches(t_token_array *tokens, size_t cursor,
		t_string_vector *entries)
{
	tokens->items[cursor].type = TOK_WORD;
	strv_destruct(entries);
}

t_token_array	*create_new_token_array(size_t new_size, size_t new_cap)
{
	t_token_array	*new_tokens_array;

	new_tokens_array = alloc(sizeof(t_token_array));
	new_tokens_array->size = new_size;
	new_tokens_array->cap = new_cap;
	new_tokens_array->items = alloc(sizeof(t_token) * new_cap);
	return (new_tokens_array);
}
