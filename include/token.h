#ifndef TOKEN_H
# define TOKEN_H
# include <t_string.h>
# include <zenint.h>
# include <t_string_vector.h>

#define OPAREN '('
#define CPAREN ')'
#define WHITESPACE      " \t\n"
#define AND             '&'
#define PIPE            '|'
#define INPUT_REDIRECT  '<'
#define OUTPUT_REDIRECT '>'
#define HEREDOC         "<<"
#define APPEND           ">>"
#define QUOTE_SINGLE    '\''
#define QUOTE_DOUBLE    '\"'
#define EXPANSION_MARK  '$'
#define EXIT_STATUS     '?'
#define SPACE           ' '
#define TAB             '\t'
#define NEWLINE         '\n'
#define ZERO_BYTE       '\0'
#define ESCAPE          '\\'
#define COMMENT         '#'
#define NOT_QUOTED      0
#define SINGLE_QUOTED   1
#define DOUBLE_QUOTED   2

/*
	// NOTE:	 Unset a bit
		byte &= ~(1 << index);
	// NOTE:	set a bit
		byte |= (1 << index)
*/
typedef enum e_parsing_state
{
	STATE_GENERAL = 0,
	STATE_IN_QUOTE_SINGLE,
	STATE_IN_QUOTE_DOUBLE,
	STATE_IN_EXPANSION,
	STATE_IN_HEREDOC
}	t_parsing_state;

typedef enum e_error
{
	OK = 0x0,
	ERROR_SYNTAX,
	ERROR_QUOTE_UNCLOSED,
	ERROR_PIPE_SYNTAX,
	ERROR_REDIRECT_SYNTAX,
	ERROR_INVALID_OPERATOR
}	t_error;

typedef enum s_token_type
{
    TOK_PIPE = 0x0,
	TOK_SYMBOL,
	TOK_AND,
    TOK_INPUT_REDIRECT,
    TOK_OUTPUT_REDIRECT,
	TOK_APPEND,
    TOK_HEREDOC,
    TOK_EXPANSION_MARK,
	TOK_LOGICAL_OR,
	TOK_LOGICAL_AND,
    TOK_NEWLINE,
    TOK_ESCAPE,
    TOK_COMMENT,
	TOK_WORD,
	TOK_OPAREN,
	TOK_CPAREN,
	TOK_EOF,
	TOK_SIZE,
} t_token_type;

typedef struct s_mask {
	u8		*items;
	size_t	cap;
	size_t	size;
}	t_mask;

typedef struct s_token
{
	t_token_type	type;
	t_mask			*mask;
	t_string		*lexeme;
}	t_token;

typedef  struct s_token_array
{
	t_token		*items;
	size_t		size;
	size_t		cap;
}	t_token_array;

t_token_array	*tok_array_construct(void);
t_token_array	copy_tokens(t_token_array tokens);
void			tok_array_expand(t_token_array *vec);
void			tok_array_print(t_token_array *array);
void			mask_expand(t_mask *mask);
void			mask_push_back(t_mask *mask, u8 item);
t_mask			*mask_construct(void);
void			mask_destroy(t_mask *mask);
void			token_push_back(t_token	*tok, char c, u8 state);
#endif // !TOKEN_H
