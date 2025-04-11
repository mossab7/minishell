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

t_ast	*create_command_node(t_token_array *args, t_redirect **redirects,
		int redirect_count)
{
	t_ast	*node;

	if (!args->size && !redirect_count)
		return (NULL);
	node = alloc(sizeof(t_ast));
	node->type = NODE_COMMAND;
	node->u_value.command = (t_command){.args = args,
		.redirects = redirects, .redirect_count = redirect_count};
	return (node);
}

void	init_command_resources(t_token_array **arg, t_redirect ***redirects,
		int *redirect_count, size_t size)
{
	*arg = tok_array_construct();
	*redirects = alloc(size * sizeof(t_redirect *));
	*redirect_count = 0;
}

void	cleanup_command_resources(t_token_array *args, t_redirect **redirects)
{
	toks_destroy(args);
	ft_free(redirects);
}

void	process_word_token(t_token token, t_token_array *args, size_t *index)
{
	tok_array_expand(args);
	args->items[args->size++] = *ft_tokdup(token);
	(*index)++;
}

t_ast	*parse_command(t_token_array *tokens, size_t *index)
{
	t_token_array	*args;
	t_redirect		**redirects;
	int				redirect_count;
	t_token			token;

	init_command_resources(&args, &redirects, &redirect_count, tokens->size);
	while (true)
	{
		token = peek_token(tokens, *index);
		if (token.type == TOK_WORD || token.type == TOK_WILD_CARD)
			process_word_token(token, args, index);
		else if (token.type >= TOK_INPUT_REDIRECT && token.type <= TOK_HEREDOC)
		{
			if (!process_redirection_token(tokens, index, redirects,
					&redirect_count))
				return ((cleanup_command_resources(args, redirects)), NULL);
		}
		else
			break ;
	}
	if (args->size == 0 && redirect_count == 0)
		return ((cleanup_command_resources(args, redirects)), NULL);
	return (create_command_node(args, redirects, redirect_count));
}
