/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:44:28 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 19:45:54 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

void	*syntax_error(const char *message)
{
	if (!check_context_flag(FLAG_SYNTAX_ERROR)
		&& !check_context_flag(FLAG_HERE_DOC_ACTIVE))
	{
		zen_elog ("Syntax error: %s\n", message);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	set_context_flag(FLAG_SYNTAX_ERROR);
	return (NULL);
}

t_token	peek_token(t_token_array *tokens, size_t index)
{
	if (index >= tokens->size)
		return ((t_token){TOK_EOF, NULL});
	return (tokens->items[index]);
}

bool	match_token(t_token_type type, t_token_array *tokens, size_t *index)
{
	if (*index >= tokens->size)
		return (false);
	if (tokens->items[*index].type == type)
	{
		(*index)++;
		return (true);
	}
	return (false);
}

t_ast	*build_ast(t_token_array *tokens)
{
	size_t	index;
	t_ast	*ast;
	t_token	token;

	index = 0;
	ast = parse_and_or(tokens, &index);
	token = peek_token(tokens, index);
	if (token.type != TOK_EOF && !check_context_flag(FLAG_SYNTAX_ERROR))
	{
		zen_elog("syntax error near unexpected token : %s\n"\
			, tokens->items[index].lexeme->cstring);
		return (NULL);
	}
	if (check_context_flag(FLAG_SYNTAX_ERROR))
		return (NULL);
	return (ast);
}

void	ast_destroy(t_ast *root)
{
	int	i;

	if (root == NULL)
		return ;
	ast_destroy(root->left);
	ast_destroy(root->right);
	i = 0;
	ft_free(root->u_value.command.args);
	i = 0;
	while (i < root->u_value.command.redirect_count)
	{
		ft_free(root->u_value.command.redirects[i]->delimiter);
		str_destruct(root->u_value.command.redirects[i]->filename);
		ft_free(root->u_value.command.redirects[i]);
		i++;
	}
	ft_free(root->u_value.command.redirects);
	ft_free(root);
}
