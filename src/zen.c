#include <zen.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <signals.h>

char *zen_prompt(t_env *env)
{
    char *user;
    char *pwd;
    char *buff;
    t_string *zen_prompt_;
    t_context *context;

    context = *get_context();
    zen_prompt_ = str_construct();
    pwd = env_get(env, "PWD");
    if (!pwd)
        pwd = "PWD_NOT_SET";
    user = env_get(env, "USER");
    if (!user)
        user = "incognito";
    if (isatty(STDIN_FILENO))
        str_join(zen_prompt_, 4, user, "@", pwd, ": ");
    else
        zen_prompt_->cstring = NULL;
    context->readline_active = 1;
	if(context->siginit_received == true)
		printf("\n");
    buff = readline(zen_prompt_->cstring);
    str_destruct(zen_prompt_);
    return (buff);
}

int main(int ac, char **av, const char *envp[])
{
    setbuf(stdout, NULL);
    setup_signal_handlers();
    t_token_array *tokens;
	t_context *context;
    char *input;
    t_lexer *lex;
    t_env *env;
    (void)ac;
    (void)av;
    env = env_parse(envp);
    while (1)
    {
        input = zen_prompt(env);
		register_memory_allocation(get_memory_tracker(),create_memory_record(input,free));
        if (!input)
            break;
		if(!*input)
			continue;
        {
			context = *get_context();
			context->readline_active = 0;
			context->siginit_received = false;
            lex = lexer_init(input);
			lex->tokens->input = input;
            int e = lexer_tokenize(lex);
            switch (e)
            {
            case ERROR_SYNTAX:
            {
				ft_free(input);
                break;
            }
            case ERROR_PIPE_SYNTAX:
            {
				ft_free(input);
                break;
            }
            case ERROR_REDIRECT_SYNTAX:
            {
				ft_free(input);
                break;
            }
            case ERROR_INVALID_OPERATOR:
            {
				ft_free(input);
                break;
            }
            case ERROR_QUOTE_UNCLOSED:
            {
                // printf("[Error]: Quote unclosed\n");
                // printf("    %s\n", lex->source);
                // printf("    %*s\n", ((int)lex->cursor), "^\n");
				ft_free(input);
                break;
            }
            case OK:
            {
                tokens = lex->tokens;
                //tok_array_print(tokens);
                expand(env, tokens);
                {
					tokens->syntax_error = false;
                    t_ast *root = build_ast(tokens);
					if(!root)
					{
						ft_free(input);
						continue;
					}
					if(tokens->syntax_error == true)
					{
						ft_free(input);
						continue;
					}
                    //print_ast(root,0);
                    execute_ast(root, env);
					add_history(tokens->input);
                }
            }
            break;
			default:
			{
				free(input);
				break;
			}
            }
        }
        //free(input);
        if (!isatty(STDIN_FILENO))
            break;
    }
    cleanup_memory_tracker(get_memory_tracker());
    return 0;
}
