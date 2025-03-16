/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zen.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:38:43 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 06:44:44 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int main(int ac, char **av, const char *envp[])
{
	t_ast	*root;
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
		{
			str_destruct(input);
			continue;
		}
		context = *get_context();
		context->readline_active = 0;
		context->siginit_received = false;
		lex = lexer_init(input->cstring);
		lex->tokens->input = input;
		lexer_status = lexer_tokenize(lex);
		if (lexer_status == ERROR_QUOTE_UNCLOSED)
		{
			// TODO: Visualize a proper error message for the quoute that was not closed
		}
		if (lexer_status == OK)
		{
			tokens = lex->tokens;
			expand(env, tokens);
			tok_array_print(tokens);
			tokens->syntax_error = false;
			root = build_ast(tokens);
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
		ast_destroy(root);
        if (!isatty(STDIN_FILENO))
            break;
    }
    cleanup_memory_tracker(get_memory_tracker());
    return 0;
}
