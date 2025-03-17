/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zen.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:38:43 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/17 16:53:19 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int main(int ac, char **av, const char *envp[])
{
	t_parser	parser;
	t_error		err;
    (void)ac;
    (void)av;

    setbuf(stdout, NULL);
    setup_signal_handlers();
	ft_memset(&parser, 0, sizeof(parser));
    parser.env = env_parse(envp);
    while (1)
    {	
		err = parser_prepare_input(&parser);
		if (err == FAILURE_)
			continue ;
		if (err == CATA_FAILURE_)
			break ;
		init_context(parser.input, parser.env);
		parser_lex(&parser);
		if (parser.lexer_status == ERROR_QUOTE_UNCLOSED)
		{
			// TODO: Visualize a proper error message for the quoute that was not closed
		}
		if (parser.lexer_status == OK)
			parser_procced_to_exec(&parser);
		parser_destroy(&parser);
        if (!isatty(STDIN_FILENO))
            break;
    }
    cleanup_memory_tracker(get_memory_tracker());
    return 0;
}
