/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:46:30 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 19:46:31 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

t_ast	*create_subshell_node(t_ast *child)
{
	t_ast	*node;

	node = alloc(sizeof(t_ast));
	node->type = NODE_SUBSHELL;
	if (child)
	{
		node->u_value.command = (t_command){.args = NULL, .argc = 0,
			.redirects = child->u_value.command.redirects,
			.redirect_count = child->u_value.command.redirect_count};
	}
	node->left = child;
	node->right = NULL;
	return (node);
}

t_ast	*init_subshell_node(t_ast *child, t_token_array *tokens)
{
	t_ast	*subshell_node;

	subshell_node = alloc(sizeof(t_ast));
	subshell_node->type = NODE_SUBSHELL;
	subshell_node->left = child;
	subshell_node->right = NULL;
	subshell_node->u_value.command.args = NULL;
	subshell_node->u_value.command.argc = 0;
	subshell_node->u_value.command.redirects = alloc(tokens->size
			* sizeof(t_redirect *));
	subshell_node->u_value.command.redirect_count = 0;
	return (subshell_node);
}

bool	collect_subshell_redirections(t_ast *subshell_node,
		t_token_array *tokens, size_t *index)
{
	t_redirect	*redir;
	t_token		token;

	while (true)
	{
		token = peek_token(tokens, *index);
		if (token.type >= TOK_INPUT_REDIRECT && token.type <= TOK_HEREDOC)
		{
			redir = parse_redirection(tokens, index);
			if (!redir)
				return (false);
			subshell_node->u_value.command.redirects[subshell_node
				->u_value.command.redirect_count++] = redir;
		}
		else
			break ;
	}
	return (true);
}

t_ast	*parse_subshell_redirections(t_token_array *tokens, size_t *index,
		t_ast *node)
{
	t_ast	*subshell_node;

	subshell_node = init_subshell_node(node, tokens);
	if (!collect_subshell_redirections(subshell_node, tokens, index))
	{
		ft_free(subshell_node->u_value.command.redirects);
		ft_free(subshell_node);
		return (NULL);
	}
	return (subshell_node);
}
