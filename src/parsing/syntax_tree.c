#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <zen.h>
#include <libft.h>
void	print_ast(t_ast *node, int level)
{
    t_redirect	*redir;

    if (node == NULL)
    {
        return ;
    }
    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }
    switch (node->type)
    {
    case NODE_COMMAND:
    {
        printf("├── COMMAND\n");
        for (int i = 0; i < level + 1; i++)
        {
            printf("    ");
        }
        printf("├── ARGS: ");
        for (int i = 0; i < node->value.command.argc; i++)
        {
            printf("%s ", node->value.command.args[i]);
        }
        printf("\n");
        for (int i = 0; i < level + 1; i++)
        {
            printf("    ");
        }
        printf("├── REDIRECTS: ");
        for (int i = 0; i < node->value.command.redirect_count; i++)
        {
            redir = node->value.command.redirects[i];
            if (redir->type == REDIR_INPUT)
            {
                printf("< %s ", redir->filename);
            }
            else if (redir->type == REDIR_OUTPUT)
            {
                printf("> %s ", redir->filename);
            }
            else if (redir->type == REDIR_APPEND)
            {
                printf(">> %s ", redir->filename);
            }
            else if (redir->type == REDIR_HEREDOC)
            {
                printf("<< %s ", redir->delimiter);
            }
        }
        printf("\n");
        break ;
    }
    case NODE_PIPE:
        printf("├── PIPE\n");
        break ;
    case NODE_SUBSHELL:
        printf("├── SUBSHELL\n");
        break ;
    case NODE_LOGICAL_AND:
        printf("├── AND\n");
        break ;
    case NODE_LOGICAL_OR:
        printf("├── OR\n");
        break ;
    default:
        printf("├── UNKNOWN NODE TYPE\n");
        break ;
    }
    if (node->left)
    {
        print_ast(node->left, level + 1);
    }
    if (node->right)
    {
        print_ast(node->right, level + 1);
    }
}

t_ast		*parse_and_or(t_token_array *tokens, size_t *index);

void	*syntax_error(const char *message,t_token_array *tokens)
{
	if(tokens->syntax_error == false && tokens->here_doc_active == false)
	{
    	fprintf(stderr, "Syntax error: %s\n", message);
    	rl_replace_line("", 0);
    	rl_on_new_line();
	}
	tokens->syntax_error = true;
	return (NULL);
}

t_ast	*create_command_node(char **args, int argc, t_redirect **redirects,
        int redirect_count)
{
    t_ast	*node;

	if(!*args)
		return (NULL);
    node = alloc(sizeof(t_ast));
    node->type = NODE_COMMAND;
    node->value.command = (t_command){
        .args = args,
        .argc = argc,
        .redirects = redirects,
        .redirect_count = redirect_count
    };
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

t_ast	*create_subshell_node(t_ast *child)
{
    t_ast	*node;

    node = alloc(sizeof(t_ast));
    node->type = NODE_SUBSHELL;
    node->left = child;
    node->right = NULL;
    return (node);
}
/*utils for heredoc*/
char *ft_mkstemp(void)
{
    long i = 0;
    char *filename;
	char *tmp;

    while(true)
    {
        tmp = ft_itoa(i);
		filename = ft_strjoin("/tmp/heredoc",tmp);
		ft_free(tmp);
        if(access(filename, F_OK) == -1)
            break;
        ft_free(filename);
        i++;
    }
    return (filename);
}

void setup_heredoc_signals(void)
{
    struct sigaction sa;
    int signals[] = {
        SIGABRT, SIGALRM, SIGBUS, SIGCHLD, SIGCONT,
        SIGFPE, SIGHUP, SIGILL, SIGPIPE, SIGPROF,
        SIGSEGV, SIGTERM, SIGTRAP, SIGTSTP, SIGTTIN,
        SIGTTOU, SIGUSR1, SIGUSR2, SIGVTALRM, SIGXCPU,
        SIGXFSZ
    };
    int i;

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;

    i = 0;
    while (i < (int)(sizeof(signals) / sizeof(signals[0])))
    {
        sigaction(signals[i], &sa, NULL);
        i++;
    }

    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

int setup_here_doc(t_redirect *redir, t_token_array *tokens)
{
    tokens->here_doc_active = true;
    char *filename = ft_mkstemp();
    int fd = open(filename, O_RDWR | O_CREAT | O_EXCL, 0600);
    pid_t pid;
    int status;
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    if (fd == -1)
    {
        perror("heredoc");
        return (-1);
    }

    pid = fork();
    if (pid == -1)
    {
        close(fd);
        unlink(filename);
        perror("fork");
        return (-1);
    }

    if (pid == 0)
    {
        setup_heredoc_signals();

        t_string *line;
		t_string *input = str_construct();
        close(pipefd[0]);
        while ((line = ft_readline("> ")) != NULL)
        {
            if (strcmp(line->cstring, redir->delimiter) == 0)
            {
                str_destruct(line);
                break;
            }
            write(fd, line->cstring, line->size);
            write(fd, "\n", 1);
			str_join(input,2,line->cstring,"\n");
            str_destruct(line);
        }
		write(pipefd[1], input->cstring,input->size);
        close(pipefd[1]);
        close(fd);
        exit(0);
    }

    close(fd);
    close(pipefd[1]);
	char *heredoc_content;
	str_append("\n",tokens->input);
	while(true)
	{
		heredoc_content = get_next_line(pipefd[0]);
		if(!heredoc_content)
			break;
		str_append(heredoc_content,tokens->input);
		ft_free(heredoc_content);
	}
    close(pipefd[0]);

    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status))
    {
        unlink(filename);
        ft_free(filename);
        return (-1);
    }
    redir->filename = filename;
    return (0);
}
/*end*/
t_redirect	*create_redirect(t_redirect_type type, char *target,t_token_array *tokens)
{
    t_redirect	*redir;

    redir = alloc(sizeof(t_redirect));
    redir->type = type;
    if (type == REDIR_HEREDOC)
    {
        redir->delimiter = target;
        setup_here_doc(redir,tokens);
        // if(redir->filename == NULL)
        //     return (NULL);
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
    {
        type = REDIR_INPUT;
    }
    else if (token.type == TOK_OUTPUT_REDIRECT)
    {
        type = REDIR_OUTPUT;
    }
    else if (token.type == TOK_APPEND)
    {
        type = REDIR_APPEND;
    }
    else if (token.type == TOK_HEREDOC)
    {
        type = REDIR_HEREDOC;
    }
    else
    {
        return (NULL);
    }
    (*index)++;
    token = peek_token(tokens, *index);
    if (token.type != TOK_WORD)
        return(syntax_error("Expected filename/delimiter after redirection",tokens));
    (*index)++;
    return (create_redirect(type, ft_strdup(token.lexeme->cstring),tokens));
}

t_ast	*parse_command(t_token_array *tokens, size_t *index)
{
    char		**args;
    t_redirect	**redirects;
    int			argc = 0;
    int			redirect_count;
    t_token		token;
    t_redirect	*redir;

    args = alloc((tokens->size + 1) * sizeof(char *));
    redirects = alloc(tokens->size * sizeof(t_redirect *));
    argc = 0, redirect_count = 0;
    while (true)
    {
        token = peek_token(tokens, *index);
        if (token.type == TOK_WORD)
        {
            args[argc++] = ft_strdup(token.lexeme->cstring);
            (*index)++;
        }
        else if (token.type >= TOK_INPUT_REDIRECT && token.type <= TOK_HEREDOC)
        {
            redir = parse_redirection(tokens, index);
			if(!redir)
			{
				while(argc > 0)
					ft_free(args[--argc]);
				ft_free(args);
				ft_free(redirects);
				return (NULL);
			}
            redirects[redirect_count++] = redir;
        }
        else
            break ;
    }
	if(argc == 0 && redirect_count == 0)
	{
		ft_free(args);
		ft_free(redirects);
		return (NULL);
	}
    return (create_command_node(args, argc, redirects, redirect_count));
}

t_ast	*parse_primary(t_token_array *tokens, size_t *index)
{
    t_ast	*node;

    node = NULL;
    if (match_token(TOK_OPAREN, tokens, index))
    {
		node = parse_and_or(tokens, index);
		if(node == NULL)
            return (syntax_error("near unexpected token `()'",tokens));
        if (!match_token(TOK_CPAREN, tokens, index))
        {
            return (syntax_error("Expected closing parenthesis",tokens));
        }
        node = create_subshell_node(node);
    }
    else
    {
        node = parse_command(tokens, index);
		if(!node && !tokens->syntax_error)
			return (syntax_error("Unexpected token",tokens));
    }
    return (node);
}

t_ast	*parse_pipe(t_token_array *tokens, size_t *index)
{
    t_ast	*left;

    left = parse_primary(tokens, index);
	if(!left)
		return (NULL);
    while (match_token(TOK_PIPE, tokens, index))
    {
        left = create_binary_node(NODE_PIPE, left, parse_primary(tokens,
                    index));
		if(!left->right)
			return (syntax_error("Invalid null command",tokens));
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
                return (syntax_error("near unexpected token `&&'",tokens));
            (*index)++;
            left = create_binary_node(NODE_LOGICAL_AND, left, parse_pipe(tokens,
                        index));
        }
        else if (token.type == TOK_LOGICAL_OR)
        {
			if (left == NULL)
				return (syntax_error("near unexpected token `||'",tokens));
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
        syntax_error("Unexpected input",tokens);
		return (NULL);
    }
	if(tokens->syntax_error == true)
		return (NULL);
    return (ast);
}

// void	ast_destroy(t_ast *root)
// {
// 	ast_destroy(root->left);
// 	ast_destroy(root->right);
// 	// TODO: Free the root's dangled pointed
// }
