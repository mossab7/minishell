/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:08:42 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/15 15:28:00 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	is_expandable(t_token_type type)
{
	return ((type == TOK_WORD) || (type == TOK_WILD_CARD));
}

void	tokens_expand(t_env *env, t_token_array *tokens, size_t *cursor)
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
		ft_free(value);
		if ((tk->lexeme->size == 0) && (key->mask->context & NOT_QUOTED))
			toks_remove(tokens, *cursor);
		str_destruct(key);
		if (tokens->size == 0 || *cursor == tokens->size)
			break ;
		tk = &(tokens->items[*cursor]);
		key = extract_key(tk->lexeme);
	}
}

t_token_array	*expand_string_to_tokens(t_string *string)
{
	t_token_array	*toks;

	toks = tok_array_construct();
	toks->items[toks->size].lexeme = string_dup(string);
	toks->items[toks->size].type = string->tmp_type;
	toks->size++;
	expand_command(get_context_env(), &toks);
	return (toks);
}
