/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_lex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:35:34 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/18 17:46:18 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static void	regulate_tok_types(t_token_array *tokens)
{
	size_t	i;

	i = 0;
	while (i < tokens->size)
		tokens->items[i++].type = TOK_WORD;
}

t_token_array	*tokenize_source(const char *source)
{
	t_lexer			*lex;
	t_token_array	*copy;

	lex = lexer_init(source);
	lexer_tokenize(lex);
	copy = tokens_copy(lex->tokens);
	lexer_destroy(lex);
	regulate_tok_types(copy);
	return (copy);
}
