/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:52:49 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 19:52:50 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

bool	process_redirection_token(t_token_array *tokens, size_t *index,
		t_redirect **redirects, int *redirect_count)
{
	t_redirect	*redir;

	redir = parse_redirection(tokens, index);
	if (!redir)
		return (false);
	redirects[*redirect_count] = redir;
	(*redirect_count)++;
	return (true);
}

t_redirect	*parse_redirection(t_token_array *tokens, size_t *index)
{
	t_redirect_type	type;
	t_token			token;

	token = peek_token(tokens, *index);
	if (token.type == TOK_INPUT_REDIRECT)
		type = REDIR_INPUT;
	else if (token.type == TOK_OUTPUT_REDIRECT)
		type = REDIR_OUTPUT;
	else if (token.type == TOK_APPEND)
		type = REDIR_APPEND;
	else if (token.type == TOK_HEREDOC)
		type = REDIR_HEREDOC;
	else
		return (NULL);
	(*index)++;
	token = peek_token(tokens, *index);
	if (token.type != TOK_WORD)
		return (syntax_error("Expected filename/delimiter after redirection"));
	(*index)++;
	return (create_redirect(type, ft_strdup(token.lexeme->cstring)));
}

t_redirect	*create_redirect(t_redirect_type type, char *target)
{
	t_redirect	*redir;

	redir = alloc(sizeof(t_redirect));
	redir->type = type;
	if (type == REDIR_HEREDOC)
	{
		redir->delimiter = target;
		setup_here_doc(redir);
	}
	else
	{
		redir->filename = target;
		redir->delimiter = NULL;
	}
	return (redir);
}
