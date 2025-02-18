#ifndef AST_H
#define AST_H

#include <lexer.h>

typedef enum {
    NODE_COMMAND,
    NODE_PIPE,
    NODE_SUBSHELL,
    NODE_LOGICAL_AND,
    NODE_LOGICAL_OR,
} t_node_type;

typedef enum {
    REDIR_INPUT,
    REDIR_OUTPUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type type;
	char	*filename;
	int		fd;
	char *delimiter;
}	t_redirect;

typedef struct s_command
{
	char	*name;
	char	**args;
	int		argc;
	struct s_redirect	**redirects;
	int		redirect_count;
}	t_command;

typedef struct s_ast
{
	t_node_type		type;
	union {
		t_command	command;
	}				value;
	struct s_ast	*left;
	struct s_ast	*right;
	int				status;
}	t_ast;

void		free_ast(t_ast *node);
t_ast		*new_node(t_ast node);
void		print_ast(t_ast *node, int level);
t_ast *build_ast(t_token_array *tokens);
t_string_vector *wildcard_expansion(char *left,char *right);
#endif // AST_H
