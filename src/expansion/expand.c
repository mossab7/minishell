#include <zen.h>

// TODO: implement the core logic of expansion
//       - Details:
//       1 - for now just, expans $Variable to the value of that Variable..

void noop(void)
{}

void	expand(t_env *env, t_token_array *tokens)
{
	char			*value;
	size_t			iter;

	value = NULL;
	iter = 0;
	while (iter < tokens->size)
	{
		if (tokens->items[iter].type != TOK_EXPANSION_MARK)
			noop();
		else {
			if (ft_strcmp(tokens->items[iter].lexeme->cstring + 1, "?") == 0)
				value = ft_itoa(env->last_command_status);
			else
				value = env_get(env, (tokens->items[iter].lexeme->cstring + 1));
			if (!*value)
			{
				ft_memmove((tokens->items + iter), (tokens->items + iter + 1), (tokens->size - iter) * sizeof(*tokens->items));
				tokens->size--;
				continue ;
			}
			tokens->items[iter].type = TOK_WORD;
			str_overwrite(value, tokens->items[iter].lexeme);
		}
		iter++;
	}
}
