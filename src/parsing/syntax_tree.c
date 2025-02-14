#include <zen.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <zen.h>
void print_ast(t_ast *node, int level) {
    if (node == NULL) {
        return;
    }

    // Print indentation
    for (int i = 0; i < level; i++) {
        printf("    ");
    }

    // Print node type and content
    switch (node->type) {
        case NODE_COMMAND: {
            printf("├── COMMAND\n");
            for (int i = 0; i < level + 1; i++) {
                printf("    ");
            }
            printf("├── ARGS: ");
            for (int i = 0; i < node->value.command.argc; i++) {
                printf("%s ", node->value.command.args[i]);
            }
            printf("\n");

            for (int i = 0; i < level + 1; i++) {
                printf("    ");
            }
            printf("├── REDIRECTS: ");
            for (int i = 0; i < node->value.command.redirect_count; i++) {
                t_redirect *redir = node->value.command.redirects[i];
                if (redir->type == REDIR_INPUT) {
                    printf("< %s ", redir->filename);
                } else if (redir->type == REDIR_OUTPUT) {
                    printf("> %s ", redir->filename);
                } else if (redir->type == REDIR_APPEND) {
                    printf(">> %s ", redir->filename);
                } else if (redir->type == REDIR_HEREDOC) {
                    printf("<< %s ", redir->delimiter);
                }
            }
            printf("\n");
            break;
        }
        case NODE_PIPE:
            printf("├── PIPE\n");
            break;
        case NODE_SUBSHELL:
            printf("├── SUBSHELL\n");
            break;
        case NODE_LOGICAL_AND:
            printf("├── AND\n");
            break;
        case NODE_LOGICAL_OR:
            printf("├── OR\n");
            break;
        default:
            printf("├── UNKNOWN NODE TYPE\n");
            break;
    }

    // Recursively print left and right children
    if (node->left) {
        print_ast(node->left, level + 1);
    }
    if (node->right) {
        print_ast(node->right, level + 1);
    }
}


t_ast *parse_and_or(t_token_array *tokens, size_t *index);

// Utility functions
void syntax_error(const char *message) {
    fprintf(stderr, "Syntax error: %s\n", message);
    exit(EXIT_FAILURE);
}

void *xalloc(size_t size) {
    void *ptr = calloc(1, size);
    if (!ptr) {
        perror("Allocation failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

char *xstrdup(const char *str) {
    char *dup = strdup(str);
    if (!dup) {
        perror("strdup failed");
        exit(EXIT_FAILURE);
    }
    return dup;
}

// AST creation functions
t_ast *create_command_node(char **args, int argc, t_redirect **redirects, int redirect_count) {
    t_ast *node = xalloc(sizeof(t_ast));
    node->type = NODE_COMMAND;
    node->value.command = (t_command){
        .args = args,
        .argc = argc,
        .redirects = redirects,
        .redirect_count = redirect_count
    };
    return node;
}

t_ast *create_binary_node(t_node_type type, t_ast *left, t_ast *right) {
    t_ast *node = xalloc(sizeof(t_ast));
    node->type = type;
    node->left = left;
    node->right = right;
    return node;
}

t_ast *create_subshell_node(t_ast *child) {
    t_ast *node = xalloc(sizeof(t_ast));
    node->type = NODE_SUBSHELL;
    node->left = child;
    node->right = NULL;
    return node;
}
t_redirect *create_redirect(t_redirect_type type, char *target) {
    t_redirect *redir = xalloc(sizeof(t_redirect));
    redir->type = type;
    if (type == REDIR_HEREDOC) {
        redir->delimiter = target;
        redir->filename = NULL;
    } else {
        redir->filename = target;
        redir->delimiter = NULL;
    }
    return redir;
}

// Parser helper functions
t_token peek_token(t_token_array *tokens, size_t index) {
    if (index >= tokens->size) return (t_token){TOK_EOF, NULL};
    return tokens->items[index];
}

bool match_token(t_token_type type, t_token_array *tokens, size_t *index) {
    if (*index >= tokens->size) return false;
    if (tokens->items[*index].type == type) {
        (*index)++;
        return true;
    }
    return false;
}

// Redirection parsing (replaced switch with if-else)
t_redirect *parse_redirection(t_token_array *tokens, size_t *index) {
    t_redirect_type type;
    t_token token = peek_token(tokens, *index);
    
    if (token.type == TOK_INPUT_REDIRECT) {
        type = REDIR_INPUT;
    } else if (token.type == TOK_OUTPUT_REDIRECT) {
        type = REDIR_OUTPUT;
    } else if (token.type == TOK_APPEND) {
        type = REDIR_APPEND;
    } else if (token.type == TOK_HEREDOC) {
        type = REDIR_HEREDOC;
    } else {
        return NULL;
    }
    (*index)++;

    token = peek_token(tokens, *index);
    if (token.type != TOK_WORD) syntax_error("Expected filename/delimiter after redirection");
    (*index)++;

    return create_redirect(type, xstrdup(token.lexeme->cstring));
}

// Command parsing with improved memory management
t_ast *parse_command(t_token_array *tokens, size_t *index) {
    char **args = alloc((tokens->size + 1) * sizeof(char *));
    t_redirect **redirects = alloc(tokens->size * sizeof(t_redirect *));
    int argc = 0, redirect_count = 0;

    while (true) {
        t_token token = peek_token(tokens, *index);
        if (token.type == TOK_WORD) {
            args[argc++] = ft_strdup(token.lexeme->cstring);
            (*index)++;
        }
        else if (token.type >= TOK_INPUT_REDIRECT && token.type <= TOK_HEREDOC) {
            t_redirect *redir = parse_redirection(tokens, index);
            redirects[redirect_count++] = redir;
        }
        else break;
    }

    // Shrink arrays to actual size
    args = realloc(args, (argc + 1) * sizeof(char *));
    redirects = realloc(redirects, redirect_count * sizeof(t_redirect *));
    return create_command_node(args, argc, redirects, redirect_count);
}



t_ast *parse_primary(t_token_array *tokens, size_t *index) {
    t_ast *node = NULL;
    
    if (peek_token(tokens, *index).type == TOK_OPAREN) {
        (*index)++; // Consume opening parenthesis
        node = parse_and_or(tokens, index);
        if (!match_token(TOK_CPAREN, tokens, index)) {
            syntax_error("Expected closing parenthesis");
        }
        // Wrap the parsed expression in a subshell node
        node = create_subshell_node(node);
    } else {
        node = parse_command(tokens, index);
    }
    
    return node;
}

t_ast *parse_pipe(t_token_array *tokens, size_t *index) {
    t_ast *left = parse_primary(tokens, index);
    while (match_token(TOK_PIPE, tokens, index)) {
        left = create_binary_node(NODE_PIPE, left, parse_primary(tokens, index));
    }
    return left;
}

t_ast *parse_and_or(t_token_array *tokens, size_t *index) {
    t_ast *left = parse_pipe(tokens, index);
    while (true) {
        if (match_token(TOK_LOGICAL_AND, tokens, index)) {
            left = create_binary_node(NODE_LOGICAL_AND, left, parse_pipe(tokens, index));
        } else if (match_token(TOK_LOGICAL_OR, tokens, index)) {
            left = create_binary_node(NODE_LOGICAL_OR, left, parse_pipe(tokens, index));
        } else break;
    }
    return left;
}

// Main parser with sequence support
t_ast *build_ast(t_token_array *tokens) {
    size_t index = 0;
    t_ast *ast = parse_and_or(tokens, &index);
    
    // Verify all tokens were consumed
    t_token token = peek_token(tokens, index);
    if (token.type != TOK_EOF) syntax_error("Unexpected input");
    
    return ast;
}

// Memory cleanup functions (added for completeness)
void free_redirect(t_redirect *redir) {
    free(redir->filename);
    free(redir->delimiter);
    free(redir);
}

void free_command(t_command *cmd) {
    int i = 0;
    while (i < cmd->argc) {
        free(cmd->args[i]);
        i++;
    }
    i = 0;
    while (i < cmd->redirect_count) {
        free_redirect(cmd->redirects[i]);
        i++;
    }
    free(cmd->args);
    free(cmd->redirects);
}

void free_ast(t_ast *ast) {
    if (!ast) return;
    free_ast(ast->left);
    free_ast(ast->right);
    if (ast->type == NODE_COMMAND) free_command(&ast->value.command);
    free(ast);
}