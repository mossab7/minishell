/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tree.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/04/20 21:12:06 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_TREE_H
# define SYNTAX_TREE_H

# include <env.h>
# include <lexer.h>
# include <t_string.h>

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
	t_string			*filename;
	int					fd;
	char				*delimiter;
	t_string			*heredoc_delimiter;
}						t_redirect;

typedef struct s_command
{
	t_token_array		*args;
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

void					print_ast(t_ast *node, int level);
void					ast_destroy(t_ast *root);
t_ast					*build_ast(t_token_array *tokens);
t_string_vector			*wildcardexpansion(t_string *pattern);
void					parser_destroy(t_parser *parser);
t_error					parser_prepare_input(t_parser *parser);
void					parser_lex(t_parser *parser);
void					parser_procced_to_exec(t_parser *parser);
t_ast					*parse_command(t_token_array *tokens, size_t *index);
t_ast					*parse_pipe(t_token_array *tokens, size_t *index);
t_ast					*parse_and_or(t_token_array *tokens, size_t *index);
t_ast					*parse_subshell_redirections(t_token_array *tokens,
							size_t *index, t_ast *node);
t_ast					*parse_primary(t_token_array *tokens, size_t *index);
char					*ft_mkstemp(void);
int						cleanup_on_error(char *filename, int fd, int status);
int						init_heredoc(t_string **filename,
							int *fd, int pipefd[2]);
t_string				*read_heredoc_content(t_redirect *redir, int fd);
void					handle_heredoc_child(int fd, int pipefd[2],
							t_redirect *redir);
void					read_from_pipe(int pipefd);
int						setup_here_doc(t_redirect *redir);
t_ast					*create_subshell_node(t_ast *child);
t_ast					*init_subshell_node(t_ast *child,
							t_token_array *tokens);
bool					collect_subshell_redirections(t_ast *subshell_node,
							t_token_array *tokens, size_t *index);
t_ast					*parse_subshell_redirections(t_token_array *tokens,
							size_t *index, t_ast *node);
t_ast					*create_command_node(t_token_array *args,
							t_redirect **redirects,
							int redirect_count);
void					init_command_resources(t_token_array **args,
							t_redirect ***redirects, int *redirect_count,
							size_t size);
void					cleanup_command_resources(t_token_array *args,
							t_redirect **redirects);
void					process_word_token(t_token token, t_token_array *args,
							size_t *index);
bool					process_redirection_token(t_token_array *tokens,
							size_t *index, t_redirect **redirects,
							int *redirect_count);
t_ast					*parse_command(t_token_array *tokens, size_t *index);
t_redirect				*create_redirect(t_redirect_type type,
							t_string *target);
t_redirect				*parse_redirection(t_token_array *tokens,
							size_t *index);
t_ast					*create_binary_node(t_node_type node_type, t_ast *left,
							t_ast *right);
t_ast					*parse_primary(t_token_array *tokens, size_t *index);
t_ast					*parse_pipe(t_token_array *tokens, size_t *index);
t_ast					*parse_and_or(t_token_array *tokens, size_t *index);
t_token					peek_token(t_token_array *tokens, size_t index);
bool					match_token(t_token_type type, t_token_array *tokens,
							size_t *index);
void					*syntax_error(const char *message);
t_ast					*process_logical_operator(t_token_type token_type,
							t_ast *left, t_token_array *tokens, size_t *index);
t_ast					*parse_and_or(t_token_array *tokens, size_t *index);
t_ast					*create_binary_node(t_node_type type, t_ast *left,
							t_ast *right);
t_ast					*parse_primary(t_token_array *tokens, size_t *index);
t_ast					*parse_pipe(t_token_array *tokens, size_t *index);
#endif // SYNTAX_TREE_H
