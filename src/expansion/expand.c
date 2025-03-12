#include <zen.h>

t_string	*extract_key(t_string *string)
{
	char		*starter_ptr;
	t_string	*key;

	starter_ptr = ft_strchr(string->cstring, '$');
	if (!starter_ptr)
		return (NULL);
	key = str_construct();
	while ((*starter_ptr != 0) && !ft_isspace(*starter_ptr))
	{
		str_push_back(key, (*starter_ptr));
		starter_ptr++;
		if (*starter_ptr == '$')
			break ;
	}
	printf("KEY EXTRACTED: %s\n", (key->cstring));
	return (key);
}

// TODO: implement the core logic of expansion
//       - Details:
//       1 - for now just, expans $Variable to the value of that Variable..
//       
void compress_spaces(char *value, size_t init_length)
{
	int iter = 0;

	if (!value || !*value)
		return ;
	while (value[iter])
	{
		while (ft_isspace(value[iter]))
			ft_memmove(value + iter, value + iter + 1, (init_length - iter));
		iter++;
		iter += ft_isspace(value[iter]);
	}
}

void	expand_value(t_token *tok, t_env *env, t_token_array *tokens)
{
	t_string	*key;
	char		*value;
	size_t		iter;

	iter = (tok - tokens->items);
	key = extract_key(tok->lexeme);
	while (key)
	{
		if (key->size == 1)
		{
			key = extract_key(tok->lexeme);
			continue ;
		}
		if (ft_strcmp(((key->cstring) + 1), "?") == 0)
			value = ft_itoa(env->last_command_status);
		else
			value = ft_strdup(env_get(env, ((key->cstring) + 1)));
		// TODO: compression..
		// if (compression)
		// 	compress_spaces(value, ft_strlen(value));
		str_substitute(tok->lexeme, value, key->cstring);
		if (!tok->lexeme->size)
		{
			ft_memmove(tok, (tok + 1), (tokens->size - iter) * sizeof(*tokens->items));
			tokens->size--;
		}
		key = extract_key(tok->lexeme);
	}
	tok->type = TOK_WORD;
}

void	expand(t_env *env, t_token_array *tokens)
{
	t_token			*tok;
	size_t			iter;

	iter = 0;
	while (iter < tokens->size)
	{
		tok = &tokens->items[iter];
		expand_value(tok, env, tokens);
		iter++;
	}
}
