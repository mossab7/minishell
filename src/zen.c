#include <zen.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>



char *zen_prompt(t_env *env)
{
	char *user;
	char *pwd;
	char *buff;
	t_string *zen_prompt_;

	pwd = env_get(env, "PWD");
	if (!pwd)
		pwd = "PWD_NOT_SET";
	user = env_get(env, "USER");
	if (!user)
		user = "incognito";
	zen_prompt_ = vstr_construct(4, user, "@", pwd, ": ");
	buff = readline(zen_prompt_->cstring);
	str_destruct(zen_prompt_);
	return (buff);
}


void	handle_signal(int signum, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if(signum == SIGINT)
	{
		write(1,"\n",1);
		rl_on_new_line();
		rl_replace_line("",0);
		rl_redisplay();
	}
}
void signal_handler()
{
	struct sigaction	set;
	sigemptyset(&set.sa_mask);
	set.sa_flags = SA_SIGINFO;
	set.sa_sigaction = handle_signal;
	sigaction(SIGINT,&set,NULL);
}

int main(int ac, char **av, const char *envp[])
{
	setbuf(stdout, NULL);
	//signal_handler();
	t_token_array *tokens;
	char *input;
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
		add_history(input);
		{
			lex = lexer_init(input);
			switch (lexer_tokenize(lex)) {
				case ERROR_SYNTAX: {} break;
				case ERROR_PIPE_SYNTAX: {} break;
				case ERROR_REDIRECT_SYNTAX: {} break;
				case ERROR_INVALID_OPERATOR: {} break;
				case ERROR_QUOTE_UNCLOSED: {
					printf("[Error]: Quote unclosed\n");
					printf("    %s\n", lex->source);
					printf("   ");
					for (size_t i = 0; i < lex->cursor; i++)
						printf(" ");
					printf("^\n");
				} break;
				case OK: {
					tokens = lex->tokens;
					/*ft_printf("Before exps: \n");*/
					/*tok_array_print(tokens);*/
					expand(env, tokens);
					/*ft_printf("After exps: \n");*/
					/*tok_array_print(tokens);*/
					{
						t_ast *root = build_ast(tokens);
						//print_ast(root,0);
						execute_ast(root, env);
					}
				} break;
			}
		}
		ft_free(lex);
		ft_free(tokens);
		ft_free(input);
	}
	cleanup_memory_tracker(get_memory_tracker());
	return 0;
}


// int main(int ac,char **av)
// {
// 	(void)ac;
// 	t_string_vector *entries = wildcardexpansion(av[1]);
// 	for(size_t i = 0;i < entries->size;i++)
// 		printf("->%s\n",entries->cstrings[i]);
// 	return 0;
// }
