/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_consumers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 05:40:31 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/18 17:34:01 by lazmoud          ###   ########.fr       */
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

static void	init_word_token(t_lexer *lex, t_token **tok, t_u8 *input_state,
		t_stack *stacks[])
{
	*tok = lex->tokens->current;
	token_next(lex->tokens);
	(*tok)->type = TOK_WORD;
	*input_state = 0;
	stacks[SINGLE_QUOTES_INDEX] = stack_construct();
	stacks[DOUBLE_QUOTES_INDEX] = stack_construct();
}

static int	process_char(t_token *tok, t_lexer *lex, t_u8 *input_state,
		t_stack *stacks[])
{
	if (is_quote(lex->source[lex->cursor]))
	{
		lex_quote(tok, input_state, lex->source[lex->cursor], stacks);
		lex->cursor++;
		return (1);
	}
	token_push_back(tok, lex->source[lex->cursor++], *input_state);
	return (0);
}

static t_error	cleanup_and_check_quotes(t_stack *stacks[])
{
	int	unmatched_exists;

	unmatched_exists = ((stacks[SINGLE_QUOTES_INDEX]->size > 0)
			|| (stacks[DOUBLE_QUOTES_INDEX]->size > 0));
	stack_destroy(stacks[DOUBLE_QUOTES_INDEX]);
	stack_destroy(stacks[SINGLE_QUOTES_INDEX]);
	if (unmatched_exists)
		return (ERROR_QUOTE_UNCLOSED);
	return (OK);
}

static void	finalize_word_token(t_token *tok)
{
	if (!tok->lexeme->size)
		token_push_back(tok, 0, 0);
	if (ft_strchr(tok->lexeme->cstring, '*'))
		tok->type = TOK_WILD_CARD;
}

t_error	consume_word(t_lexer *lex)
{
	t_stack	*stacks[QUOTING_STACKS_SIZE];
	t_u8		input_state;
	t_token	*tok;
	t_error	status;
	int		processed_quote;

	init_word_token(lex, &tok, &input_state, stacks);
	while (is_valid_word_character(lex->source[lex->cursor], input_state))
	{
		processed_quote = process_char(tok, lex, &input_state, stacks);
		if (processed_quote)
			continue ;
	}
	status = cleanup_and_check_quotes(stacks);
	if (status != OK)
		return (status);
	finalize_word_token(tok);
	return (OK);
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
