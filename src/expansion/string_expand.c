/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:20:30 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 21:12:06 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static int	find_dollar_sign(t_string *string)
{
	if (string->cursor < 0)
		return (0);
	string->cursor = ft_strchr(
			(string->cstring + string->cursor), '$') - string->cstring;
	if (string->cursor < 0)
		return (0);
	if (string->mask->items[string->cursor] == SINGLE_QUOTED)
		return (0);
	return (1);
}

static t_string	*build_key(t_string *string, t_u8 context)
{
	t_string	*key;

	key = str_construct();
	str_push_back(key, string->cstring[string->cursor++]);
	while (string->cstring[string->cursor] != 0
		&& !ft_isspace(string->cstring[string->cursor])
		&& string->cstring[string->cursor] != '/')
	{
		if (key->size > 1 && !ft_isalnum(string->cstring[string->cursor]))
			break ;
		str_push_back(key, string->cstring[string->cursor]);
		string->cursor++;
		if (context != string->mask->items[string->cursor])
			break ;
		if (string->cstring[string->cursor] == '$')
			break ;
	}
	key->mask->context = context;
	return (key);
}

static t_string	*handle_solo_dollar(t_string *string, t_string *key)
{
	if (key->size == 1)
	{
		string->cursor++;
		str_destruct(key);
		return (extract_key(string));
	}
	return (key);
}

t_string	*extract_key(t_string *string)
{
	t_u8		context;
	t_string	*key;

	if (!string->size)
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
