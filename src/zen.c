/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zen.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:38:43 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/23 18:03:17 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	main(int ac, char **av, const char *envp[])
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
		{
			parser_destroy(&parser);
			continue ;
		}
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
		if (!isatty(STDIN_FILENO))
			break ;
		parser_destroy(&parser);
	}
	cleanup_memory_tracker(get_memory_tracker());
	return (0);
}

// int main(int ac, char **av, const char *envp[])
// {
// 	(void)ac;
// 	(void)envp;
// 	t_string_vector *entries = strv_construct();
// 	if (!entries)
// 		return (1);
// 	entries = wildcardexpansion(av[1]);
// 	for (size_t i = 0; i < entries->size; i++)
// 	{
// 		printf("->%s\n",entries->cstrings[i]);
// 	}
// 	strv_destruct(entries);
// 	return (0);
// }
