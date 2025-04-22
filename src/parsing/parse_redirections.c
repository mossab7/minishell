/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:52:49 by mbouhia           #+#    #+#             */
/*   Updated: 2025/04/20 21:11:41 by lazmoud          ###   ########.fr       */
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
	if (token.type != TOK_WORD && token.type != TOK_WILD_CARD)
		return (syntax_error("Expected filename/delimiter after redirection"));
	(*index)++;
	token.lexeme->tmp_type = token.type;
	return (create_redirect(type, token.lexeme));
}

t_redirect	*create_redirect(t_redirect_type type, t_string *target)
{
	t_redirect	*redir;

	redir = alloc(sizeof(t_redirect));
	redir->type = type;
	if (type == REDIR_HEREDOC)
	{
		string_expand(get_context_env(), target);
		redir->heredoc_delimiter = target;
		if (!check_context_flag(FLAG_SIGINT_RECEIVED))
			setup_here_doc(redir);
	}
	else
	{
		redir->filename = string_dup(target);
		redir->delimiter = NULL;
	}
	return (redir);
}
