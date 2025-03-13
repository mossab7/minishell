/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:29:42 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/12 17:46:24 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

#define NOT_QUOTED      0
#define SINGLE_QUOTED   1
#define DOUBLE_QUOTED   2
static t_string	*extract_key(t_string *string)
{
	int			cursor;
	u8			context;
	t_string	*key;


	if (!string->size)
		return (NULL);
	cursor = str_search(string, "$");
	if (cursor < 0)
		return (NULL);
	context = string->mask->items[cursor];
	if (context == SINGLE_QUOTED)
		return (NULL);
	key = str_construct();
	while (string->cstring[cursor] != 0 && !ft_isspace(string->cstring[cursor]))
	{
		str_push_back(key, string->cstring[cursor]);
		cursor++;
		if (string->cstring[cursor] == '$' || context != string->mask->items[cursor])
			break ;
	}
	key->mask->context = context;
	return (key);
}

void	string_expand(t_env *env, t_token *tok, t_token_array *tokens)
{
	t_string	*key;
	char		*value;
	int			iter;

	iter = (tok - tokens->items);
	key = extract_key(tok->lexeme);
	while (key)
	{
		if (key->size == 1)
		{
			str_destruct(key);
			key = extract_key(tok->lexeme);
			continue ;
		}
		if (ft_strcmp(((key->cstring) + 1), "?") == 0)
			value = ft_itoa(env->last_command_status);
		else
			value = ft_strdup(env_get(env, ((key->cstring) + 1)));
		str_substitute(tok->lexeme, value, key->cstring);
		// TODO: Check the key's context, if it is double qourted, no splitting must be done.
		// if it is a normal key which is not wrapped between quotations then just split the fields as new tokens.
		// Use the lexer to do so..
		if (key->mask->context == NOT_QUOTED)
		{
			// ... "1 2 3" ...
			// ... ...
			// ... 1 2 3 ...
			t_token_array *fields = tokenize_source((const char *)tok->lexeme->cstring);

			printf("SPLIT\n");
			printf("======================================================\n");
			tok_array_print(fields);
			printf("======================================================\n");
			ft_memmove(tokens->items + iter, tokens->items + iter + 1, (tokens->size - iter) * sizeof(*tokens->items));
			tokens->size--;
			while (fields->size + tokens->size >= tokens->cap)
				tok_array_expand_anyhow(tokens);
			ft_memmove(tokens->items + iter + fields->size, tokens->items + iter, (tokens->size - iter) * sizeof(*tokens->items));
			ft_memcpy(tokens->items + iter, fields->items, fields->size * sizeof(*tokens->items));
			tokens->size += fields->size;
		}
		ft_free(value);
		str_destruct(key);
		key = extract_key(tok->lexeme);
	}
}
