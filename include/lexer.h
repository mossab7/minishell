/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 18:37:43 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <libft.h>
# include <stdbool.h>
# include <stdio.h>
# include <token.h>
# define QUOTING_STACKS_SIZE 2
# define SINGLE_QUOTES_INDEX 0
# define DOUBLE_QUOTES_INDEX 1

typedef struct s_lexer
{
	const char		*source;
	size_t			cursor;
	t_token_array	*tokens;
}					t_lexer;

t_token_array		*tokenize_source(const char *source);
t_lexer				*lexer_init(const char *source);
t_error				lexer_tokenize(t_lexer *lex);
t_token_array		*simple_export_tokenizer(char *segment);
t_error				consume_word(t_lexer *lex);
t_error				consume_symbol(t_lexer *lex);
char				*get_type_as_cstr(t_token_type type);
void				lexer_destroy(t_lexer *lex);
int					ft_is_operator(int c);
int					ft_zen_isalnum(int c);
int					is_inside_quotes(t_u8 state);
int					is_valid_word_character(int c, t_u8 state);
#endif // !LEXER_H
