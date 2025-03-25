/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:49:33 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 19:49:34 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

t_ast	*create_command_node(t_arg arg, t_redirect **redirects,
		int redirect_count)
{
	t_ast	*node;

	if (!*arg.args && !redirect_count)
		return (NULL);
	node = alloc(sizeof(t_ast));
	node->type = NODE_COMMAND;
	node->u_value.command = (t_command){.args = arg.args, .argc = arg.argc,
		.redirects = redirects, .redirect_count = redirect_count};
	return (node);
}

void	init_command_resources(t_arg *arg, t_redirect ***redirects,
		int *redirect_count, size_t size)
{
	arg->args = alloc((size + 1) * sizeof(char *));
	*redirects = alloc(size * sizeof(t_redirect *));
	arg->argc = 0;
	*redirect_count = 0;
}

void	cleanup_command_resources(t_arg *arg, t_redirect **redirects)
{
	while (arg->argc > 0)
		ft_free(arg->args[--arg->argc]);
	ft_free(arg->args);
	ft_free(redirects);
}

void	process_word_token(t_token token, t_arg *arg, size_t *index)
{
	arg->args[arg->argc] = ft_strdup(token.lexeme->cstring);
	arg->argc++;
	(*index)++;
}

t_ast	*parse_command(t_token_array *tokens, size_t *index)
{
	t_arg		arg;
	t_redirect	**redirects;
	int			redirect_count;
	t_token		token;

	init_command_resources(&arg, &redirects, &redirect_count, tokens->size);
	while (true)
	{
		token = peek_token(tokens, *index);
		if (token.type == TOK_WORD)
			process_word_token(token, &arg, index);
		else if (token.type >= TOK_INPUT_REDIRECT && token.type <= TOK_HEREDOC)
		{
			if (!process_redirection_token(tokens, index, redirects,
					&redirect_count))
				return ((cleanup_command_resources(&arg, redirects)), NULL);
		}
		else
			break ;
	}
	if (arg.argc == 0 && redirect_count == 0)
		return ((cleanup_command_resources(&arg, redirects)), NULL);
	return (create_command_node(arg, redirects, redirect_count));
}
