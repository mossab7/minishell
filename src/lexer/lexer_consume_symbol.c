/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_consume_symbol.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:48:57 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 19:50:17 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static t_error	check_next(t_token *tok, t_lexer *lex, t_token_type pair_type)
{
	if (lex->source[lex->cursor - 1] == lex->source[lex->cursor])
	{
		tok->type = pair_type;
		token_push_back(tok, lex->source[lex->cursor++], NOT_QUOTED);
	}
	return (OK);
}

static t_error	consume_sym_pair(t_token *tok, t_lexer *lex, t_token_type type,
		t_token_type pair_type)
{
	tok->type = type;
	token_push_back(tok, lex->source[lex->cursor++], NOT_QUOTED);
	return (check_next(tok, lex, pair_type));
}

static t_error	consume_parenthesis(t_token *tok, t_lexer *lex, char symbol)
{
	if (symbol == OPAREN)
		tok->type = TOK_OPAREN;
	else
		tok->type = TOK_CPAREN;
	token_push_back(tok, symbol, NOT_QUOTED);
	lex->cursor++;
	return (OK);
}

static t_error	consume_generic_symbol(t_token *tok, t_lexer *lex)
{
	tok->type = TOK_SYMBOL;
	token_push_back(tok, lex->source[lex->cursor++], NOT_QUOTED);
	return (OK);
}

t_error	consume_symbol(t_lexer *lex)
{
	t_token	*tok;
	char	current;

	tok = lex->tokens->current;
	token_next(lex->tokens);
	current = lex->source[lex->cursor];
	if (current == PIPE)
		return (consume_sym_pair(tok, lex, TOK_PIPE, TOK_LOGICAL_OR));
	if (current == AND)
		return (consume_sym_pair(tok, lex, TOK_AND, TOK_LOGICAL_AND));
	if (current == INPUT_REDIRECT)
		return (consume_sym_pair(tok, lex, TOK_INPUT_REDIRECT, TOK_HEREDOC));
	if (current == OUTPUT_REDIRECT)
		return (consume_sym_pair(tok, lex, TOK_OUTPUT_REDIRECT, TOK_APPEND));
	if (current == OPAREN || current == CPAREN)
		return (consume_parenthesis(tok, lex, current));
	return (consume_generic_symbol(tok, lex));
}
