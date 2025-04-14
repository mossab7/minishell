/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_export_tokenizer.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:20:32 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 17:01:45 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static void	consume_if(t_lexer *lex, int (*pred)(int))
{
	str_push_back(lex->tokens->current->lexeme, lex->source[lex->cursor++]);
	while (pred(lex->source[lex->cursor]))
		str_push_back(lex->tokens->current->lexeme, lex->source[lex->cursor++]);
	token_next(lex->tokens);
}

static int	parse_variable_name(t_lexer *lex, t_token_array *token_array)
{
	if (!ft_isalpha(lex->source[lex->cursor]))
		return (0);
	token_array->current->type = TOK_WORD;
	consume_if(lex, ft_isalnum_or_underscore);
	return (1);
}

static int	parse_equals_operator(t_lexer *lex, t_token_array *token_array)
{
	if (ft_strncmp(lex->source + lex->cursor, "=", 1) == 0)
	{
		token_array->current->type = TOK_EQ;
		str_push_back(token_array->current->lexeme, lex->source[lex->cursor++]);
	}
	else if (ft_strncmp(lex->source + lex->cursor, "+=", 2) == 0)
	{
		token_array->current->type = TOK_PEQ;
		str_push_back(token_array->current->lexeme, lex->source[lex->cursor++]);
		str_push_back(token_array->current->lexeme, lex->source[lex->cursor++]);
	}
	else
		return (0);
	token_next(token_array);
	return (1);
}

static void	parse_value(t_lexer *lex, t_token_array *token_array)
{
	token_array->current->type = TOK_WORD;
	consume_if(lex, is_not_null);
}

t_token_array	*simple_export_tokenizer(char *segment)
{
	t_lexer			*lex;
	t_token_array	*token_array;

	lex = lexer_init(segment);
	token_array = lex->tokens;
	if (!parse_variable_name(lex, token_array))
	{
		lexer_destroy(lex);
		return (NULL);
	}
	if (lex->source[lex->cursor])
	{
		if (!parse_equals_operator(lex, token_array))
		{
			lexer_destroy(lex);
			return (NULL);
		}
		parse_value(lex, token_array);
	}
	ft_free(lex);
	return (token_array);
}
