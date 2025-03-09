#include <zen.h>

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

void	expand_value(t_token *tok, t_env *env, t_token_array *tokens, u8 compression)
{
	char *value;
	size_t iter;
	char *exp;

	exp = tok->lexeme->cstring;
	iter = (tok - tokens->items);
	if (!compression)
		exp = ft_strchr(tok->lexeme->cstring, '$');
	if (ft_strcmp(exp + 1, "?") == 0)
		value = ft_itoa(env->last_command_status);
	else
		value = env_get(env, (tok->lexeme->cstring + 1));
	if (!*value)
	{
		ft_memmove(tok, (tok + 1), (tokens->size - iter) * sizeof(*tokens->items));
		tokens->size--;
		return ;
	}
	tok->type = TOK_WORD;
	str_overwrite(value, tok->lexeme);
	if (compression)
		compress_spaces(tok->lexeme->cstring, tok->lexeme->size);
}

void	expand(t_env *env, t_token_array *tokens)
{
	char			*exp;
	t_token			*tok;
	size_t			iter;

	iter = 0;
	while (iter < tokens->size)
	{
		tok = &tokens->items[iter];
		if(tok->type == TOK_EXPANSION_MARK)
			expand_value(tok, env, tokens, 1);
		else
		{
			exp = ft_strchr(tok->lexeme->cstring, '$');
			if (exp && !tok->mask->items[exp - tok->lexeme->cstring])
			{
				tok->type = TOK_EXPANSION_MARK;
				continue ;
			}
			if (exp && tok->mask->items[exp - tok->lexeme->cstring] != SINGLE_QUOTED)
				expand_value(tok, env, tokens, 0);
		}
		iter++;
	}
}
