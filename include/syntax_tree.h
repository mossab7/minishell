/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tree.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/24 19:43:41 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_TREE_H
# define SYNTAX_TREE_H

# include <env.h>
# include <lexer.h>

# define DL "warning: here-document delimited by end-of-file (wanted `%s')\n"

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_SUBSHELL,
	NODE_LOGICAL_AND,
	NODE_LOGICAL_OR,
}						t_node_type;

typedef enum e_redirect_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				*filename;
	int					fd;
	char				*delimiter;
}						t_redirect;

typedef struct s_command
{
	char				*name;
	char				**args;
	int					argc;
	struct s_redirect	**redirects;
	int					redirect_count;
}						t_command;

typedef struct s_ast
{
	t_node_type			type;
	union
	{
		t_command		command;
	} u_value;
	struct s_ast		*left;
	struct s_ast		*right;
	int					status;
}						t_ast;

typedef struct s_parser
{
	t_ast				*tree;
	t_lexer				*lexer;
	t_env				*env;
	t_error				lexer_status;
	t_string			*input;
}						t_parser;

typedef struct s_arg
{
	char				**args;
	int					argc;
}						t_arg;

void					free_ast(t_ast *node);
t_ast					*new_node(t_ast node);
void					print_ast(t_ast *node, int level);
void					ast_destroy(t_ast *root);
t_ast					*build_ast(t_token_array *tokens);
t_string_vector			*wildcardexpansion(char *pattern);
void					parser_destroy(t_parser *parser);
t_error					parser_prepare_input(t_parser *parser);
void					parser_lex(t_parser *parser);
void					parser_procced_to_exec(t_parser *parser);
#endif // SYNTAX_TREE_H
