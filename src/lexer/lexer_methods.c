/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_methods.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 05:40:25 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 05:46:34 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

t_lexer *lexer_init(const char *source)
{
	t_lexer	*lex;

	lex = alloc(sizeof(*lex));
	lex->source = source;
	lex->cursor = 0;
	lex->tokens = tok_array_construct();

	return (lex);
}

t_token_array	*tokenize_source(const char *source)
{
	t_lexer	*lex = lexer_init(source);

	lexer_tokenize(lex);
	ft_free(lex);
	return (lex->tokens);
}

t_error lexer_tokenize(t_lexer *lex)
{
    t_error err;

    err = OK;
    while (lex->source[lex->cursor] != 0)
    {
        while (ft_isspace(lex->source[lex->cursor]))
            lex->cursor++;
        if (ft_zen_isalnum(lex->source[lex->cursor]) || is_quote(lex->source[lex->cursor]))
            err = consume_word(lex);
        else if (ft_ispunct(lex->source[lex->cursor]))
            err = consume_symbol(lex);
        if (err != OK)
			break ;
        while (ft_isspace(lex->source[lex->cursor]))
            lex->cursor++;
    }
	lex->tokens->current->type = TOK_EOF;
	return (err);
}

void	lexer_destroy(t_lexer *lex)
{
	if (lex)
	{
		// printf("lex = %p\n", lex);
		toks_destroy(lex->tokens);
		ft_free(lex);
	}
}
