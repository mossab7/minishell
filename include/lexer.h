#ifndef LEXER_H
# define LEXER_H

# include <stdio.h>
# include <libft.h>
# include <stdbool.h>
# include <token.h>
# define QUOTING_STACKS_SIZE 2
# define SINGLE_QUOTES_INDEX 0
# define DOUBLE_QUOTES_INDEX 1

typedef struct s_lexer
{
	const char		*source;
	size_t			cursor;
	t_token_array	*tokens;
}	t_lexer;

t_lexer *lexer_init(const char *source);
t_error	lexer_tokenize(t_lexer *lex);
char	*get_type_as_cstr(t_token_type type);
t_error consume_word(t_token *tok, t_lexer *lex);
t_error consume_string_literal(t_token *tok, t_lexer *lex);
t_error	check_next(t_token *tok, t_lexer *lex, t_token_type pair_type);
t_error consume_symbol(t_token *tok, t_lexer *lex);
t_error consume_sym_pair(t_token *tok, t_lexer *lex, t_token_type type,t_token_type pair_type);
#endif // !LEXER_H
