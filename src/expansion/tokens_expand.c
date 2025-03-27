/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:08:42 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 20:24:36 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	find_next_expansion(t_token_array *tokens, size_t *cursor)
{
	*cursor = 0;
	while (*cursor < tokens->size
		&& (!ft_strchr(tokens->items[*cursor].lexeme->cstring, '$')
			|| (tokens->items[*cursor].lexeme->size == 1)))
		(*cursor)++;
	while (*cursor < tokens->size
		&& (tokens->items[*cursor].type != TOK_WORD))
		(*cursor)++;
}

static int	must_field_split(t_string *key, t_token *tk, int is_export)
{
	return (key->mask->context == NOT_QUOTED
		&& !is_export && !ft_strchr(tk->lexeme->cstring, '='));
}

void	tokens_expand(t_env *env, t_token_array *tokens,
				size_t *cursor, int is_export)
{
	t_string	*key;
	char		*value;
	t_token		*tk;

	tk = &(tokens->items[*cursor]);
	key = extract_key(tk->lexeme);
	while (key && *cursor < tokens->size && tokens->size)
	{
		if (ft_strcmp(((key->cstring) + 1), "?") == 0)
			value = ft_itoa(env->last_command_status);
		else
			value = ft_strdup(env_get(env, ((key->cstring) + 1)));
		str_substitute(tk->lexeme, value, key);
		if (must_field_split(key, tk, is_export))
		{
			tokens_field_split(tokens, *cursor);
			find_next_expansion(tokens, cursor);
		}
		ft_free(value);
		str_destruct(key);
		if (tokens->size == 0 || *cursor == tokens->size)
			break ;
		tk = &(tokens->items[*cursor]);
		key = extract_key(tk->lexeme);
	}
}

int	is_expandable(t_token_type type)
{
	return ((type == TOK_WORD) || (type == TOK_WILD_CARD));
}
