/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:20:30 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/28 15:19:05 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static int	find_dollar_sign(t_string *string)
{
	if (lexeme_ended(string))
		return (0);
	string->cursor = str_search(string, "$", string->cursor);
	while (is_dollar_sign_not_expandable(string, string->cursor))
	{
		string->cursor++;
		string->cursor = str_search(string, "$", string->cursor);
	}
	if (lexeme_ended(string) || string->cursor < 0)
		return (0);
	return (1);
}

static t_string	*build_key(t_string *string, t_u8 context)
{
	t_string		*key;
	unsigned int	id;

	key = str_construct();
	key->mask->context = context;
	id = string->mask->ids[string->cursor];
	str_push_back(key, string->cstring[string->cursor++]);
	while (!lexeme_ended(string)
		&& !ft_isspace(string->cstring[string->cursor])
		&& string->cstring[string->cursor] != '/'
		&& string->mask->items[string->cursor] == context
		&& string->mask->ids[string->cursor] == id)
	{
		if (key->size > 1 && !ft_isalnum(string->cstring[string->cursor])
			&& string->cstring[string->cursor] != '_')
			break ;
		str_push_back(key, string->cstring[string->cursor]);
		string->cursor++;
		if ((string->cstring[string->cursor - 1] == '$') && (key->size == 2))
			break ;
		if (lexeme_ended(string)
			|| (context != string->mask->items[string->cursor]))
			break ;
	}
	return (key);
}

static t_string	*handle_solo_dollar(t_string *string, t_string *key)
{
	if (key->size == 1)
	{
		if (string->cursor
			&& string->cstring[string->cursor] != 0
			&& string->mask->items[string->cursor - 1] & NOT_QUOTED)
		{
			str_shift_left(string, (string->cursor - 1), 1);
			string->cstring[string->size] = 0;
		}
		else
			string->mask->items[string->cursor - 1] |= EXPANDED;
		str_destruct(key);
		return (extract_key(string));
	}
	return (key);
}

t_string	*extract_key(t_string *string)
{
	t_u8		context;
	t_string	*key;

	if (lexeme_ended(string) || !string->size)
		return (NULL);
	if (!find_dollar_sign(string))
		return (NULL);
	context = string->mask->items[string->cursor];
	key = build_key(string, context);
	return (handle_solo_dollar(string, key));
}

void	string_expand(t_env *env, t_string *string)
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
