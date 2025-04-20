/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 05:40:10 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 05:40:13 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static void	lex_single(t_token *tok, t_u8 *state, char quote,
		t_stack *stacks[QUOTING_STACKS_SIZE])
{
	if (*state & DOUBLE_QUOTED)
		token_push_back(tok, quote, *state);
	if (stacks[SINGLE_QUOTES_INDEX]->size)
	{
		stack_pop(stacks[SINGLE_QUOTES_INDEX]);
		if (*state & SINGLE_QUOTED)
			*state = 0;
	}
	else if (!(*state & DOUBLE_QUOTED))
	{
		if (!(*state & DOUBLE_QUOTED)
			&& (stacks[DOUBLE_QUOTES_INDEX]->size == 0))
			*state |= SINGLE_QUOTED;
		stack_push_back(stacks[SINGLE_QUOTES_INDEX], quote);
	}
}

static void	lex_double(t_token *tok, t_u8 *state, char quote,
		t_stack *stacks[QUOTING_STACKS_SIZE])
{
	if (*state & SINGLE_QUOTED)
		token_push_back(tok, quote, *state);
	if (stacks[DOUBLE_QUOTES_INDEX]->size)
	{
		stack_pop(stacks[DOUBLE_QUOTES_INDEX]);
		if (*state & DOUBLE_QUOTED)
			*state = 0;
	}
	else if (!(*state & SINGLE_QUOTED))
	{
		if (!(*state & SINGLE_QUOTED)
			&& (stacks[SINGLE_QUOTES_INDEX]->size == 0))
			*state |= DOUBLE_QUOTED;
		stack_push_back(stacks[DOUBLE_QUOTES_INDEX], quote);
	}
}

void	lex_quote(t_token *tok, t_u8 *state, char quote,
		t_stack *stacks[QUOTING_STACKS_SIZE])
{
	if (quote == QUOTE_SINGLE)
	{
		tok->lexeme->mask->context |= (SINGLE_QUOTED);
		lex_single(tok, state, quote, stacks);
	}
	else
	{
		tok->lexeme->mask->context |= (DOUBLE_QUOTED);
		lex_double(tok, state, quote, stacks);
	}
}
