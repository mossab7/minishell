/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:36:48 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/28 14:36:54 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	parser_procced_to_exec(t_parser *parser)
{
	parser->tree = build_ast(parser->lexer->tokens);
	if (parser->lexer->tokens->size == 0)
	{
		add_history(parser->input->cstring);
		return ;
	}
	if (parser->tree && !check_context_flag(FLAG_SYNTAX_ERROR))
		execute_ast(parser->tree, parser->env);
	add_history(parser->input->cstring);
}

void	parser_lex(t_parser *parser)
{
	parser->lexer = lexer_init(parser->input->cstring);
	parser->lexer_status = lexer_tokenize(parser->lexer);
}

t_error	parser_prepare_input(t_parser *parser)
{
	parser->input = zen_prompt(parser->env);
	if (!parser->input)
		return (CATA_FAILURE_);
	if (!*parser->input->cstring)
		return (FAILURE_);
	return (OK);
}

void	parser_destroy(t_parser *parser)
{
	lexer_destroy(parser->lexer);
	ast_destroy(parser->tree);
	str_destruct(parser->input);
	parser->input = NULL;
	parser->tree = NULL;
	parser->lexer = NULL;
}
