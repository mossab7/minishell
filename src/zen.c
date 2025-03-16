/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zen.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:38:43 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 06:38:45 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

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
    t_token_array *tokens;
	t_error lexer_status;
	t_context *context;
    t_string *input;
    t_lexer *lex;
    t_env *env;
    (void)ac;
    (void)av;

    setbuf(stdout, NULL);
    setup_signal_handlers();
    env = env_parse(envp);
    while (1)
    {
        input = zen_prompt(env);
        if (!input)
            break;
		if(!*input->cstring)
			continue;
		context = *get_context();
		context->readline_active = 0;
		context->siginit_received = false;
		lex = lexer_init(input->cstring);
		lex->tokens->input = input;
		lexer_status = lexer_tokenize(lex);
		if (lexer_status == OK)
		{
			tokens = lex->tokens;
			expand(env, tokens);
			tok_array_print(tokens);
			tokens->syntax_error = false;
			t_ast *root = build_ast(tokens);
			if(!root)
				add_history(tokens->input->cstring);
			else if(tokens->syntax_error == true)
				add_history(tokens->input->cstring);
			else
			{
				execute_ast(root, env);
				add_history(tokens->input->cstring);
			}
		}
		lexer_destroy(lex);
        if (!isatty(STDIN_FILENO))
            break;
    }
    cleanup_memory_tracker(get_memory_tracker());
    return 0;
}
