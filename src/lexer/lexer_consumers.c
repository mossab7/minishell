/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_consumers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 05:40:31 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 14:49:11 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static t_error check_next(t_token *tok, t_lexer *lex, t_token_type pair_type)
{
	if (lex->source[lex->cursor - 1] == lex->source[lex->cursor])
	{
		tok->type = pair_type;
		token_push_back(tok, lex->source[lex->cursor++], NOT_QUOTED);
	}
	return (OK);
}

static t_error consume_sym_pair(t_token *tok, t_lexer *lex, t_token_type type,t_token_type pair_type)
{
	tok->type = type;
	token_push_back(tok, lex->source[lex->cursor++], NOT_QUOTED);
	return (check_next(tok, lex, pair_type));
}

t_error consume_word(t_lexer *lex)
{
	t_stack		*stacks[QUOTING_STACKS_SIZE];
	int			unmatched_exists;
	u8			input_state;
	t_token		*tok;

	tok = lex->tokens->current;
	token_next(lex->tokens);
	tok->type = TOK_WORD;
	input_state = 0;
	stacks[SINGLE_QUOTES_INDEX] = stack_construct();
	stacks[DOUBLE_QUOTES_INDEX]	= stack_construct();
	while (is_valid_word_character(lex->source[lex->cursor], input_state))
	{
		if (is_quote(lex->source[lex->cursor])) {
			lex_quote(tok, &input_state, lex->source[lex->cursor], stacks);
			lex->cursor++;
			continue ;
		}
		token_push_back(tok, lex->source[lex->cursor++], input_state);
	}
	unmatched_exists = ((stacks[SINGLE_QUOTES_INDEX]->size > 0) || (stacks[DOUBLE_QUOTES_INDEX]->size > 0));
	stack_destroy(stacks[DOUBLE_QUOTES_INDEX]);
	stack_destroy(stacks[SINGLE_QUOTES_INDEX]);
	if (unmatched_exists)
		return (ERROR_QUOTE_UNCLOSED);
	if (!tok->lexeme->size)
		token_push_back(tok, 0, 0);
	return (OK);
}

t_error consume_symbol(t_lexer *lex)
{

	t_token *tok;

	tok = lex->tokens->current;
	token_next(lex->tokens);
	if (lex->source[lex->cursor] == PIPE) // |
		return (consume_sym_pair(tok, lex, TOK_PIPE, TOK_LOGICAL_OR));
	if (lex->source[lex->cursor] == AND) // &
		return (consume_sym_pair(tok, lex, TOK_AND, TOK_LOGICAL_AND));
	if (lex->source[lex->cursor] == INPUT_REDIRECT)// <
		return (consume_sym_pair(tok, lex, TOK_INPUT_REDIRECT, TOK_HEREDOC));
	if (lex->source[lex->cursor] == OUTPUT_REDIRECT) // >
		return (consume_sym_pair(tok, lex, TOK_OUTPUT_REDIRECT, TOK_APPEND));
	if (lex->source[lex->cursor] == OPAREN)
	{
		tok->type = TOK_OPAREN;
		token_push_back(tok, lex->source[lex->cursor++], NOT_QUOTED);
		return (OK);
	}
	if (lex->source[lex->cursor] == CPAREN)
	{
		tok->type = TOK_CPAREN;
		token_push_back(tok, lex->source[lex->cursor++], NOT_QUOTED);
		return (OK);
	}
	tok->type = TOK_SYMBOL;
	token_push_back(tok, lex->source[lex->cursor++], NOT_QUOTED);
	return (OK);
}
