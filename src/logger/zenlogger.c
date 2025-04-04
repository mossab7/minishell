/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zenlogger.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:43:55 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 06:43:57 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	zen_elog(const char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ft_fprintf(STDERR_FILENO, "zen: ");
	return (ft_vfprintf(STDERR_FILENO, fmt, args));
}

void	ft_quote_error(t_parser *parser)
{
	ft_fprintf(STDERR_FILENO, "Quote was not closed properly at:\n");
	ft_fprintf(STDERR_FILENO, "%s\n", parser->lexer->source);
	while (!is_quote(parser->lexer->source[parser->lexer
				->cursor]) && parser->lexer->cursor > 0)
		parser->lexer->cursor--;
	ppad(parser->lexer->cursor, ' ', NULL);
	ft_fprintf(STDERR_FILENO, "^\n");
	add_history(parser->input->cstring);
}
