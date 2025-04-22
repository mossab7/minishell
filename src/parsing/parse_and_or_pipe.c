/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_or_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:07:16 by mbouhia           #+#    #+#             */
/*   Updated: 2025/04/22 11:11:54 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

t_ast	*process_logical_operator(t_token_type token_type, t_ast *left,
		t_token_array *tokens, size_t *index)
{
	t_node_type	node_type;
	t_ast		*right;

	if (left == NULL)
		return (set_context_flag(FLAG_SYNTAX_ERROR), NULL);
	(*index)++;
	if (token_type == TOK_LOGICAL_AND)
		node_type = NODE_LOGICAL_AND;
	else
		node_type = NODE_LOGICAL_OR;
	right = parse_pipe(tokens, index);
	if (!right)
		return (left);
	return (create_binary_node(node_type, left, right));
}

t_ast	*parse_and_or(t_token_array *tokens, size_t *index)
{
	t_ast	*left;
	t_token	token;

	left = parse_pipe(tokens, index);
	while (left && true)
	{
		token = peek_token(tokens, *index);
		if (token.type == TOK_LOGICAL_AND || token.type == TOK_LOGICAL_OR)
			left = process_logical_operator(token.type, left, tokens, index);
		else
			break ;
	}
	return (left);
}

t_ast	*create_binary_node(t_node_type type, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = alloc(sizeof(t_ast));
	node->type = type;
	node->left = left;
	node->right = right;
	return (node);
}

t_ast	*parse_primary(t_token_array *tokens, size_t *index)
{
	t_ast	*node;

	node = NULL;
	if (match_token(TOK_OPAREN, tokens, index))
	{
		node = parse_and_or(tokens, index);
		if (node == NULL)
			return (set_context_flag(FLAG_SYNTAX_ERROR), NULL);
		if (!match_token(TOK_CPAREN, tokens, index))
			return (syntax_error("unclosed parenthesis"));
		node = parse_subshell_redirections(tokens, index, node);
	}
	else
	{
		node = parse_command(tokens, index);
		if (!node && !check_context_flag(FLAG_SYNTAX_ERROR))
		{
			zen_elog("syntax error near unexpected token: %s\n",
				tokens->items[*index - 1].lexeme->cstring);
			return (set_context_flag(FLAG_SYNTAX_ERROR), NULL);
		}
	}
	return (node);
}

t_ast	*parse_pipe(t_token_array *tokens, size_t *index)
{
	t_ast	*left;

	left = parse_primary(tokens, index);
	if (!left)
		return (NULL);
	while (match_token(TOK_PIPE, tokens, index))
	{
		left = create_binary_node(NODE_PIPE, left, parse_primary(tokens,
					index));
		if (!left->right)
			return (set_context_flag(FLAG_SYNTAX_ERROR), NULL);
	}
	return (left);
}
