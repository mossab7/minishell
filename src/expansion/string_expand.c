/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:29:42 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/23 17:09:31 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static	void	find_next_expansion(t_token_array *tokens, size_t *cursor)
{
	*cursor = 0;
	while (!ft_strchr(tokens->items[*cursor].lexeme->cstring, '$') && *cursor < tokens->size)
		(*cursor)++;
}

static t_string	*extract_key(t_string *string)
{
	u8			context;
	t_string	*key;

	if (!string->size
		|| (size_t)string->cursor >= string->size || !string->cstring[string->cursor])
		return (NULL);
	string->cursor = str_search_using_cursor(string, "$");
	if (string->cursor < 0)
		return (NULL);
	context = string->mask->items[string->cursor];
	if (context == SINGLE_QUOTED)
		return (NULL);
	key = str_construct();
	while (string->cstring[string->cursor] != 0 && !ft_isspace(string->cstring[string->cursor]))
	{
		str_push_back(key, string->cstring[string->cursor]);
		string->cursor++;
		if (context != string->mask->items[string->cursor])
			break ;
		if (string->cstring[string->cursor] == '$')
			break ;
	}
	key->mask->context = context;
	if (key->size == 1)
	{
		string->cursor++;
		str_destruct(key);
		return (extract_key(string));
	}
	return (key);
}

void	_string_expand(t_env *env, t_string *string)
{
	t_string	*key;
	char		*value;

	key = extract_key(string);
	while (key)
	{
		if (ft_strcmp(((key->cstring) + 1), "?") == 0)
			value = ft_itoa(env->last_command_status);
		else
			value = ft_strdup(env_get(env, ((key->cstring) + 1)));
		str_substitute(string, value, key);
		ft_free(value);
		str_destruct(key);
		key = extract_key(string);
	}
}


void	string_expand(t_env *env, t_token_array *tokens, size_t *cursor)
{
	t_string	*key;
	char		*value;
	t_token		*tk;

	tk = &(tokens->items[*cursor]);
	key = extract_key(tk->lexeme);
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
