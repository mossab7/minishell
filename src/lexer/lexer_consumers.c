/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_consumers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 05:40:31 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 19:50:12 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

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
	if (ft_strchr(tok->lexeme->cstring, '*') && !ft_strchr(tok->lexeme->cstring, '$'))
		tok->type = TOK_WILD_CARD;
}

t_error	consume_word(t_lexer *lex)
{
	t_stack	*stacks[QUOTING_STACKS_SIZE];
	t_u8	input_state;
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
