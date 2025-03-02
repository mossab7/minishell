#include <zen.h>

int	ft_is_operator(int c)
{
	return ((c == PIPE)
		|| (c == AND)
		|| (c == INPUT_REDIRECT)
		|| (c == OUTPUT_REDIRECT)
		|| (c == OPAREN)
		|| (c == CPAREN)
		|| (c == EXPANSION_MARK));
}

int	ft_zen_isalnum(int c)
{
	if (!c)
		return (0);
	return (!ft_isspace(c) && (ft_isalnum(c) || !ft_is_operator(c)));
}

t_lexer *lexer_init(const char *source)
{
	t_lexer	*lex;

	lex = alloc(sizeof(*lex));
	lex->source = source;
	lex->cursor = 0;
	lex->tokens = tok_array_construct();

	return (lex);
}

// TODO: Get anything as a word, except already knows operators in bash. yes: kss^423@ is just a word but kss^|skks is 3 words because it has a known
// bash symbol
t_error lexer_tokenize(t_lexer *lex)
{
    t_error err;
    t_token *tok;

    err = OK;
    while (lex->source[lex->cursor] != '\0')
    {
        tok_array_expand(lex->tokens);
        tok = (lex->tokens->items + lex->tokens->size);
        tok->lexeme = str_construct();
        while (ft_isspace(lex->source[lex->cursor]))
            lex->cursor++;
        if (ft_zen_isalnum(lex->source[lex->cursor]) || is_quote(lex->source[lex->cursor]))
            err = consume_word(tok, lex);
        else if (ft_ispunct(lex->source[lex->cursor]))
            err = consume_symbol(tok, lex);
        if (err != OK)
			break ;
        while (ft_isspace(lex->source[lex->cursor]))
            lex->cursor++;
    }
	tok_array_expand(lex->tokens);
	tok = (lex->tokens->items + lex->tokens->size);
	tok->type = TOK_EOF;
	tok->lexeme = str_construct();
	return (err);
}

char *get_type_as_cstr(t_token_type type)
{
	char *them[TOK_SIZE] = {
		"PIPE",
		"SYMBOL",
		"AND",
		"INPUT_REDIRECT",
		"OUTPUT_REDIRECT",
		"HEREDOC",
		"APPEND",
		"EXPANSION_MARK",
		"LOGICAL_OR",
		"LOGICAL_AND",
		"NEWLINE",
		"ESCAPE",
		"COMMENT",
		"WORD",
		"OPAREN",
		"CPAREN",
		"OEF"
	};
	return (them[type]);
}

void	handle_quote(t_token *tok, u8 *state, char quote, t_stack *stacks[QUOTING_STACKS_SIZE])
{
	if (quote == QUOTE_SINGLE)
	{
		if (*state & DOUBLE_QUOTED)
			str_push_back(tok->lexeme, quote);
		if (stacks[SINGLE_QUOTES_INDEX]->size)
		{
			stack_pop(stacks[SINGLE_QUOTES_INDEX]);
			if (*state & SINGLE_QUOTED)
				*state = 0;
		}
		else if (!(*state & DOUBLE_QUOTED))
		{
			if (!(*state & DOUBLE_QUOTED) && (stacks[DOUBLE_QUOTES_INDEX]->size == 0))
				*state |= SINGLE_QUOTED;
			stack_push_back(stacks[SINGLE_QUOTES_INDEX], quote);
		}
	} else {
		if (*state & SINGLE_QUOTED)
			str_push_back(tok->lexeme, quote);
		if (stacks[DOUBLE_QUOTES_INDEX]->size)
		{
			stack_pop(stacks[DOUBLE_QUOTES_INDEX]);
			if (*state & DOUBLE_QUOTED)
				*state = 0;
		}
		else if (!(*state & SINGLE_QUOTED))
		{
			if (!(*state & SINGLE_QUOTED) && (stacks[SINGLE_QUOTES_INDEX]->size == 0))
				*state |= DOUBLE_QUOTED;
			stack_push_back(stacks[DOUBLE_QUOTES_INDEX], quote);
		}
	}
}

int	is_inside_quotes(u8 state)
{
	return ((state & SINGLE_QUOTED) || (state & DOUBLE_QUOTED));
}

t_error consume_word(t_token *tok, t_lexer *lex)
{
	t_stack *stacks[QUOTING_STACKS_SIZE];
	int			unmatched_exists;
	u8			input_state;

	tok->type = TOK_WORD;
	input_state = 0;
	stacks[SINGLE_QUOTES_INDEX] = stack_construct();
	stacks[DOUBLE_QUOTES_INDEX]	= stack_construct();
	while (((ft_zen_isalnum(lex->source[lex->cursor])
		|| is_quote(lex->source[lex->cursor]))) ||
		(is_inside_quotes(input_state)
		&& lex->source[lex->cursor]))
	{
		if (is_quote(lex->source[lex->cursor])) {
			handle_quote(tok, &input_state, lex->source[lex->cursor], stacks);
			lex->cursor++;
			continue ;
		}
		str_push_back(tok->lexeme, lex->source[lex->cursor++]);
	}
	unmatched_exists = ((stacks[SINGLE_QUOTES_INDEX]->size > 0) || (stacks[DOUBLE_QUOTES_INDEX]->size > 0));
	stack_destroy(stacks[DOUBLE_QUOTES_INDEX]);
	stack_destroy(stacks[SINGLE_QUOTES_INDEX]);
	if (unmatched_exists)
		return (ERROR_QUOTE_UNCLOSED);
	lex->tokens->size++;
	return (OK);
}

t_error check_next(t_token *tok, t_lexer *lex, t_token_type pair_type)
{
	if (lex->source[lex->cursor - 1] == lex->source[lex->cursor])
	{
		tok->type = pair_type;
		str_push_back(tok->lexeme, lex->source[lex->cursor++]);
	}
	return (OK);
}

t_error consume_sym_pair(t_token *tok, t_lexer *lex, t_token_type type,t_token_type pair_type)
{
	tok->type = type;
	str_push_back(tok->lexeme, lex->source[lex->cursor++]);
	return (check_next(tok, lex, pair_type));
}

t_error consume_symbol(t_token *tok, t_lexer *lex)
{
	lex->tokens->size++;
	if (lex->source[lex->cursor] == PIPE) // |
		return (consume_sym_pair(tok, lex, TOK_PIPE, TOK_LOGICAL_OR));
	if (lex->source[lex->cursor] == AND) // &
		return (consume_sym_pair(tok, lex, TOK_AND, TOK_LOGICAL_AND));
	if (lex->source[lex->cursor] == INPUT_REDIRECT)// <
		return (consume_sym_pair(tok, lex, TOK_INPUT_REDIRECT, TOK_HEREDOC));
	if (lex->source[lex->cursor] == OUTPUT_REDIRECT) // >
		return (consume_sym_pair(tok, lex, TOK_OUTPUT_REDIRECT, TOK_APPEND));
	if(lex->source[lex->cursor] == EXPANSION_MARK) // $
	{
		tok->type = TOK_EXPANSION_MARK;
		{
			str_push_back(tok->lexeme, lex->source[lex->cursor++]); // Push $
			while (ft_zen_isalnum(lex->source[lex->cursor]))
				str_push_back(tok->lexeme, lex->source[lex->cursor++]); // push rest.
		}
		return (OK);
	}
	if (lex->source[lex->cursor] == OPAREN)
	{
		tok->type = TOK_OPAREN;
		str_push_back(tok->lexeme, lex->source[lex->cursor++]);
		return (OK);
	}
	if (lex->source[lex->cursor] == CPAREN)
	{
		tok->type = TOK_CPAREN;
		str_push_back(tok->lexeme, lex->source[lex->cursor++]);
		return (OK);
	}
	tok->type = TOK_SYMBOL;
	str_push_back(tok->lexeme, lex->source[lex->cursor++]);
	return (OK);
}
