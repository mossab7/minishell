/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:44:28 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/18 16:44:57 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

t_ast		*parse_and_or(t_token_array *tokens, size_t *index);

void	*syntax_error(const char *message)
{
	if (!check_context_flag(FLAG_SYNTAX_ERROR)
		&& !check_context_flag(FLAG_HERE_DOC_ACTIVE))
	{
		fprintf(stderr, "Syntax error: %s\n", message);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	set_context_flag(FLAG_SYNTAX_ERROR);
	return (NULL);
}

t_ast	*create_command_node(t_arg arg, t_redirect **redirects,
		int redirect_count)
{
	t_ast	*node;

	if (!*arg.args && !redirect_count)
		return (NULL);
	node = alloc(sizeof(t_ast));
	node->type = NODE_COMMAND;
	node->u_value.command = (t_command){.args = arg.args, .argc = arg.argc,
		.redirects = redirects, .redirect_count = redirect_count};
	return (node);
}

t_ast	*create_binary_node(t_node_type type, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = alloc(sizeof(t_ast));
	node->type = type;
	node->left = left;
	node->right = right;
	return (node);
}

char	*ft_mkstemp(void)
{
	long	i;
	char	*filename;
	char	*tmp;

	i = 0;
	while (true)
	{
		tmp = ft_itoa(i);
		filename = ft_strjoin("/tmp/heredoc", tmp);
		ft_free(tmp);
		if (access(filename, F_OK) == -1)
			break ;
		ft_free(filename);
		i++;
	}
	return (filename);
}

static int	init_heredoc(char **filename, int *fd, int pipefd[2])
{
    *filename = ft_mkstemp();
    *fd = open(*filename, O_RDWR | O_CREAT | O_EXCL, 0600);
    
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    if (*fd == -1)
    {
        perror("heredoc");
        return (-1);
    }
    return (0);
}

static void	handle_heredoc_child(int fd, int pipefd[2], t_redirect *redir)
{
    t_string	*line;
    t_string	*input;

    setup_heredoc_signals();
    input = str_construct();
    close(pipefd[0]);
    while ((line = ft_readline("> ")) != NULL)
    {
        if (strcmp(line->cstring, redir->delimiter) == 0)
        {
            str_destruct(line);
            break;
        }
        _string_expand(get_context_env(), line);
        write(fd, line->cstring, line->size);
        write(fd, "\n", 1);
        str_join(input, 2, line->cstring, "\n");
        str_destruct(line);
    }
    write(pipefd[1], input->cstring, input->size);
    close(pipefd[1]);
    close(fd);
    str_destruct(input);
    exit(0);
}

static void	read_from_pipe(int pipefd)
{
    char	*heredoc_content;
	t_string	*input;

	input = get_context_input();
    str_append("\n", input);
    while (true)
    {
        heredoc_content = get_next_line(pipefd);
        if (!heredoc_content)
            break;
        str_append(heredoc_content, input);
        ft_free(heredoc_content);
    }
}

static int	cleanup_on_error(char *filename, int fd)
{
    if (fd >= 0)
        close(fd);
    if (filename)
    {
        unlink(filename);
        ft_free(filename);
    }
	set_context_flag(FLAG_SYNTAX_ERROR);
    return (-1);
}

int	setup_here_doc(t_redirect *redir)
{
    int			fd;
    pid_t		pid;
    int			status;
    int			pipefd[2];

    set_context_flag(FLAG_HERE_DOC_ACTIVE);
    if (init_heredoc(&redir->filename, &fd, pipefd) == -1)
        return (-1);
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (cleanup_on_error(redir->filename, fd));
    }
    if (pid == 0)
        handle_heredoc_child(fd, pipefd, redir);
    close(fd);
    close(pipefd[1]);
    read_from_pipe(pipefd[0]);
    close(pipefd[0]);
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status))
        return (cleanup_on_error(redir->filename, -1));
    return (0);
}

t_redirect	*create_redirect(t_redirect_type type, char *target)
{
	t_redirect	*redir;

	redir = alloc(sizeof(t_redirect));
	redir->type = type;
	if (type == REDIR_HEREDOC)
	{
		redir->delimiter = target;
		setup_here_doc(redir);
	}
	else
	{
		redir->filename = target;
		redir->delimiter = NULL;
	}
	return (redir);
}

t_token	peek_token(t_token_array *tokens, size_t index)
{
	if (index >= tokens->size)
		return ((t_token){TOK_EOF, NULL});
	return (tokens->items[index]);
}

bool	match_token(t_token_type type, t_token_array *tokens, size_t *index)
{
	if (*index >= tokens->size)
		return (false);
	if (tokens->items[*index].type == type)
	{
		(*index)++;
		return (true);
	}
	return (false);
}

t_redirect	*parse_redirection(t_token_array *tokens, size_t *index)
{
	t_redirect_type	type;
	t_token			token;

	token = peek_token(tokens, *index);
	if (token.type == TOK_INPUT_REDIRECT)
		type = REDIR_INPUT;
	else if (token.type == TOK_OUTPUT_REDIRECT)
		type = REDIR_OUTPUT;
	else if (token.type == TOK_APPEND)
		type = REDIR_APPEND;
	else if (token.type == TOK_HEREDOC)
		type = REDIR_HEREDOC;
	else
		return (NULL);
	(*index)++;
	token = peek_token(tokens, *index);
	if (token.type != TOK_WORD)
		return (syntax_error("Expected filename/delimiter after redirection"));
	(*index)++;
	return (create_redirect(type, ft_strdup(token.lexeme->cstring)));
}

static void	init_command_resources(t_arg *arg, t_redirect ***redirects,
	 int *redirect_count, size_t size)
{
    arg->args = alloc((size + 1) * sizeof(char *));
    *redirects = alloc(size * sizeof(t_redirect *));
    arg->argc = 0;
    *redirect_count = 0;
}

static void	cleanup_command_resources(t_arg *arg, t_redirect **redirects)
{
    while (arg->argc > 0)
        ft_free(arg->args[--arg->argc]);
    ft_free(arg->args);
    ft_free(redirects);
}

static void	process_word_token(t_token token, t_arg *arg, size_t *index)
{
    arg->args[arg->argc] = ft_strdup(token.lexeme->cstring);
    arg->argc++;
    (*index)++;
}

static bool	process_redirection_token(t_token_array *tokens, size_t *index,
                                t_redirect **redirects, int *redirect_count)
{
    t_redirect *redir;
    
    redir = parse_redirection(tokens, index);
    if (!redir)
        return (false);
    redirects[*redirect_count] = redir;
    (*redirect_count)++;
    return (true);
}

t_ast	*parse_command(t_token_array *tokens, size_t *index)
{
	t_arg		arg;
    t_redirect	**redirects;
    int			redirect_count;
    t_token		token;

    init_command_resources(&arg, &redirects, &redirect_count, tokens->size);
    while (true)
    {
        token = peek_token(tokens, *index);
        if (token.type == TOK_WORD)
            process_word_token(token, &arg, index);
        else if (token.type >= TOK_INPUT_REDIRECT && token.type <= TOK_HEREDOC)
        {
            if (!process_redirection_token(tokens, index, redirects, &redirect_count))
                return ((cleanup_command_resources(&arg, redirects)),NULL);
        }
        else
            break;
    }
    
    if (arg.argc == 0 && redirect_count == 0)
		return ((cleanup_command_resources(&arg, redirects)),NULL);
    return (create_command_node(arg, redirects, redirect_count));
}

t_ast	*create_subshell_node(t_ast *child)
{
	t_ast	*node;

	node = alloc(sizeof(t_ast));
	node->type = NODE_SUBSHELL;
	if (child)
	{
		node->u_value.command = (t_command){.args = NULL, .argc = 0,
			.redirects = child->u_value.command.redirects,
			.redirect_count = child->u_value.command.redirect_count};
	}
	node->left = child;
	node->right = NULL;
	return (node);
}

t_ast	*parse_subshell_redirections(t_token_array *tokens, size_t *index,
		t_ast *node)
{
	t_redirect	*redir;
	t_token		token;
	t_ast		*subshell_node;

	subshell_node = alloc(sizeof(t_ast));
	subshell_node->type = NODE_SUBSHELL;
	subshell_node->left = node;
	subshell_node->right = NULL;
	subshell_node->u_value.command.args = NULL;
	subshell_node->u_value.command.argc = 0;
	subshell_node->u_value.command.redirects = alloc(tokens->size
			* sizeof(t_redirect *));
	subshell_node->u_value.command.redirect_count = 0;
	while (true)
	{
		token = peek_token(tokens, *index);
		if (token.type >= TOK_INPUT_REDIRECT && token.type <= TOK_HEREDOC)
		{
			redir = parse_redirection(tokens, index);
			if (!redir)
			{
				ft_free(subshell_node->u_value.command.redirects);
				ft_free(subshell_node);
				return (NULL);
			}
			subshell_node->u_value.command.redirects[subshell_node->u_value.command.redirect_count++] = redir;
		}
		else
			break ;
	}
	return (subshell_node);
}

t_ast	*parse_primary(t_token_array *tokens, size_t *index)
{
	t_ast	*node;

	node = NULL;
	if (match_token(TOK_OPAREN, tokens, index))
	{
		node = parse_and_or(tokens, index);
		if (node == NULL)
			return (syntax_error("near unexpected token `()'"));
		if (!match_token(TOK_CPAREN, tokens, index))
		{
			return (syntax_error("Expected closing parenthesis"));
		}
		node = parse_subshell_redirections(tokens, index, node);
	}
	else
	{
		node = parse_command(tokens, index);
		if (!node && !check_context_flag(FLAG_SYNTAX_ERROR))
			return (syntax_error("Unexpected token"));
	}
	return (node);
}

t_ast	*parse_pipe(t_token_array *tokens, size_t *index)
{
	t_ast	*left;

	left = parse_primary(tokens, index);
	if (!left)
		return (NULL);
	while (match_token(TOK_PIPE, tokens, index))
	{
		left = create_binary_node(NODE_PIPE, left, parse_primary(tokens,
					index));
		if (!left->right)
			return (syntax_error("Invalid null command"));
	}
	return (left);
}

t_ast	*parse_and_or(t_token_array *tokens, size_t *index)
{
	t_ast	*left;
	t_token	token;

	left = parse_pipe(tokens, index);
	while (true)
	{
		token = peek_token(tokens, *index);
		if (token.type == TOK_LOGICAL_AND)
		{
			if (left == NULL)
				return (syntax_error("near unexpected token `&&'"));
			(*index)++;
			left = create_binary_node(NODE_LOGICAL_AND, left, parse_pipe(tokens,
						index));
		}
		else if (token.type == TOK_LOGICAL_OR)
		{
			if (left == NULL)
				return (syntax_error("near unexpected token `||'"));
			(*index)++;
			left = create_binary_node(NODE_LOGICAL_OR, left, parse_pipe(tokens,
						index));
		}
		else
			break ;
	}
	return (left);
}

t_ast	*build_ast(t_token_array *tokens)
{
	size_t	index;
	t_ast	*ast;
	t_token	token;

	index = 0;
	ast = parse_and_or(tokens, &index);
	token = peek_token(tokens, index);
	if (token.type != TOK_EOF)
	{
		syntax_error("Unexpected input");
		return (NULL);
	}
	if (check_context_flag(FLAG_SYNTAX_ERROR))
		return (NULL);
	return (ast);
}

void	ast_destroy(t_ast *root)
{
	if (root == NULL)
		return ;
	ast_destroy(root->left);
	ast_destroy(root->right);
	for (int i = 0; i < root->u_value.command.argc; i++)
		ft_free(root->u_value.command.args[i]);
	ft_free(root->u_value.command.args);
	for (int i = 0; i < root->u_value.command.redirect_count; i++)
	{
		ft_free(root->u_value.command.redirects[i]->delimiter);
		ft_free(root->u_value.command.redirects[i]->filename);
		ft_free(root->u_value.command.redirects[i]);
	}
	ft_free(root->u_value.command.redirects);
	ft_free(root);
}
