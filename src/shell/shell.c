/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 22:17:30 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/16 22:22:11 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static t_error	process_command(t_parser *parser)
{
	t_error	err;

	err = parser_prepare_input(parser);
	if (err == FAILURE_)
	{
		parser_destroy(parser);
		return (FAILURE_);
	}
	if (err == CATA_FAILURE_)
		return (CATA_FAILURE_);
	init_context(parser->input, parser->env);
	parser_lex(parser);
	if (parser->lexer_status == ERROR_QUOTE_UNCLOSED)
		ft_quote_error(parser);
	if (parser->lexer_status == OK)
		parser_procced_to_exec(parser);
	return (OK);
}

void	initialize_shell(t_parser *parser,
					const char *envp[], const char *program)
{
	setup_signal_handlers();
	ft_memset(parser, 0, sizeof(*parser));
	parser->env = env_parse(envp, program);
	init_context(parser->input, parser->env);
}

void	shell_loop(t_parser *parser)
{
	t_error	err;

	while (1)
	{
		err = process_command(parser);
		if (err == FAILURE_)
			continue ;
		if (err == CATA_FAILURE_)
			break ;
		if (!isatty(STDIN_FILENO))
			break ;
		parser_destroy(parser);
	}
}
