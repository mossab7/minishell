/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:22:59 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/14 22:58:37 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	is_field_split_character(t_string *lexeme)
{
	return ((lexeme->mask->items[lexeme->cursor] & EXPANDED)
		&& (lexeme->mask->items[lexeme->cursor] & NOT_QUOTED));
}

int	lexeme_ended(t_string *lexeme)
{
	return (lexeme->cursor >= (int)lexeme->size);
}

void	consume_non_splitable(t_token *dst, t_token *src)
{
	while (!lexeme_ended(src->lexeme)
		&& (!is_field_split_character(src->lexeme)
			|| !ft_isspace(src->lexeme->cstring[src->lexeme->cursor])))
	{
		token_push_back(dst, src->lexeme->cstring[src->lexeme->cursor],
			src->lexeme->mask->items[src->lexeme->cursor]);
		src->lexeme->cursor++;
	}
}

void	consume_into_token(t_token_array *dest_array, t_token *tk)
{
	t_token			tmp_token;

	tmp_token.lexeme = str_construct();
	tmp_token.type = tk->type;
	if (!lexeme_ended(tk->lexeme) && is_field_split_character(tk->lexeme))
	{
		while (!lexeme_ended(tk->lexeme)
			&& ft_isspace(tk->lexeme->cstring[tk->lexeme->cursor]))
			tk->lexeme->cursor++;
		while (!lexeme_ended(tk->lexeme)
			&& is_field_split_character(tk->lexeme))
		{
			token_push_back(&tmp_token,
				tk->lexeme->cstring[tk->lexeme->cursor],
				tk->lexeme->mask->items[tk->lexeme->cursor]);
			tk->lexeme->cursor++;
			if (ft_isspace(tk->lexeme->cstring[tk->lexeme->cursor]))
				break ;
		}
	}
	consume_non_splitable(&tmp_token, tk);
	tok_array_push_back(dest_array, &tmp_token);
	str_destruct(tmp_token.lexeme);
}

void	field_split(t_token_array **tokens_array, int is_export)
{
	t_token_array	*tokens;
	t_token			*tk;
	t_token_array	*new_tokens;
	size_t			i;

	new_tokens = tok_array_construct();
	tokens = *tokens_array;
	i = 0;
	while (i < tokens->size)
	{
		tk = (&tokens->items[i]);
		tk->lexeme->cursor = 0;
		if (tk->lexeme->size == 0
			|| (is_export && ft_strchr(tk->lexeme->cstring, '=')))
		{
			tok_array_push_back(new_tokens, tk);
			i++;
			continue ;
		}
		while (!lexeme_ended(tk->lexeme))
			consume_into_token(new_tokens, tk);
		i++;
	}
	toks_destroy(*tokens_array);
	*tokens_array = new_tokens;
}
