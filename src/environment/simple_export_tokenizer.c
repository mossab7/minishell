/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_export_tokenizer.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:20:32 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/15 17:03:02 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	is_not_null(int c)
{
	return (c != 0);
}

static	void	consume_if(t_lexer *lex, int (*pred)(int))
{
	str_push_back(lex->tokens->current->lexeme, lex->source[lex->cursor++]);
	while (pred(lex->source[lex->cursor]))
		str_push_back(lex->tokens->current->lexeme, lex->source[lex->cursor++]);
	token_next(lex->tokens);
}

t_token_array *simple_export_tokenizer(char *segment)
{
	t_lexer	*lex;
	t_token_array *token_array;

	lex = lexer_init(segment);
	token_array = lex->tokens;
	if (!ft_isalpha(lex->source[lex->cursor]))
	{
		lexer_destroy(lex);
		return (NULL);
	}
	token_array->current->type = TOK_WORD;
	consume_if(lex, ft_isalnum);
	if (lex->source[lex->cursor])
	{
		if (ft_strncmp(lex->source + lex->cursor, "=", 1) == 0)
		{
			token_array->current->type = TOK_EQ;
			str_push_back(token_array->current->lexeme, lex->source[lex->cursor++]);
		}
		else if (ft_strncmp(lex->source + lex->cursor, "+=", 2) == 0)
		{
			token_array->current->type = TOK_PEQ;
			str_push_back(token_array->current->lexeme, lex->source[lex->cursor++]); // ADD +
			str_push_back(token_array->current->lexeme, lex->source[lex->cursor++]); // ADD =
		} else {
			lexer_destroy(lex);
			return (NULL);
		}
		token_next(token_array);
		token_array->current->type = TOK_WORD;
		consume_if(lex, is_not_null);
	}
	ft_free(lex);
	return (token_array);
}
