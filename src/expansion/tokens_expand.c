/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:08:42 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 20:14:46 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static void	find_next_expansion(t_token_array *tokens, size_t *cursor)
{
	*cursor = 0;
	while (!ft_strchr(tokens->items[*cursor].lexeme->cstring, '$')
		&& *cursor < tokens->size)
		(*cursor)++;
}

void	tokens_expand(t_env *env, t_token_array *tokens, size_t *cursor)
{
	t_string	*key;
	char		*value;
	t_token		*tk;

	tk = &(tokens->items[*cursor]);
	key = extract_key(tk->lexeme);
	if (key)
		printf("KEY: %s\n", key->cstring);
	while (key && *cursor <= tokens->size && tokens->size)
	{
		if (ft_strcmp(((key->cstring) + 1), "?") == 0)
			value = ft_itoa(env->last_command_status);
		else
			value = ft_strdup(env_get(env, ((key->cstring) + 1)));
		str_substitute(tk->lexeme, value, key);
		if (key->mask->context == NOT_QUOTED)
		{
			tokens_field_split(tokens, *cursor);
			find_next_expansion(tokens, cursor);
		}
		ft_free(value);
		str_destruct(key);
		if (tokens->size == 0)
			break ;
		tk = &(tokens->items[*cursor]);
		key = extract_key(tk->lexeme);
	}
}
