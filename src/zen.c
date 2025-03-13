// ADD INTO THE PROJECCT: 
//		git@vogsphere-v2-bg.1337.ma:vogsphere/intra-uuid-fde9219e-75d2-42fa-a2c6-634c008f5a19-6159724-lazmoud
#include <zen.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <signals.h>

t_string *zen_prompt(t_env *env)
{
    char *user;
    char *pwd;
    t_string *buff;
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
	if (context->siginit_received == true)
		printf("\n");
    buff = ft_readline(zen_prompt_->cstring);
    str_destruct(zen_prompt_);
    return (buff);
}

int main(int ac, char **av, const char *envp[])
{
    setbuf(stdout, NULL);
    setup_signal_handlers();
    t_token_array *tokens;
	t_context *context;
    t_string *input;
    t_lexer *lex;
    t_env *env;
    (void)ac;
    (void)av;
    env = env_parse(envp);
    while (1)
    {
        input = zen_prompt(env);
        if (!input)
            break;
		if(!*input->cstring)
			continue;
        {
			context = *get_context();
			context->readline_active = 0;
			context->siginit_received = false;
            lex = lexer_init(input->cstring);
			lex->tokens->input = input;
            int e = lexer_tokenize(lex);
            switch (e)
            {
            case ERROR_SYNTAX:
            {
				str_destruct(input);
                break;
            }
            case ERROR_PIPE_SYNTAX:
            {
				str_destruct(input);
                break;
            }
            case ERROR_REDIRECT_SYNTAX:
            {
				str_destruct(input);
                break;
            }
            case ERROR_INVALID_OPERATOR:
            {
				str_destruct(input);
                break;
            }
            case ERROR_QUOTE_UNCLOSED:
            {
				str_destruct(input);
                break;
            }
            case OK:
            {
                tokens = lex->tokens;
				printf("Before: \n");
                tok_array_print(tokens);
					expand(env, tokens);
				printf("After: \n");
                tok_array_print(tokens);
                {
					tokens->syntax_error = false;
                    t_ast *root = build_ast(tokens);
					if(!root)
					{
						add_history(tokens->input->cstring);
						str_destruct(input);
						continue;
					}
					if(tokens->syntax_error == true)
					{
						add_history(tokens->input->cstring);
						str_destruct(input);
						continue;
					}
                    //print_ast(root,0);
                    execute_ast(root, env);
					add_history(tokens->input->cstring);
                }
            }
            break;
			default:
			{
				str_destruct(input);
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
