/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:40:42 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 15:55:49 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	tok_array_expand_anyhow(t_token_array *vec)
{
	size_t	i;

	vec->cap *= VEC_GROWTH_FAC;
	vec->items = ft_realloc(vec->items,
			vec->cap * sizeof(t_token), vec->size * sizeof(t_token));
	i = vec->size;
	while (i < vec->cap)
		vec->items[i++].lexeme = str_construct();
}

void	tok_array_expand(t_token_array *vec)
{
	if (vec->size >= vec->cap)
		tok_array_expand_anyhow(vec);
}

t_token_array	*tok_array_construct(void)
{
	t_token_array	*vec;
	size_t			i;

	vec = alloc(sizeof(*vec));
	vec->items = alloc(VEC_INIT_CAP * sizeof(t_token));
	vec->cap = VEC_INIT_CAP;
	vec->size = 0;
	i = 0;
	while (i < vec->cap)
	{
		vec->items[i].lexeme = str_construct();
		vec->items[i].type = TOK_NONE;
		i++;
	}
	vec->current = (vec->items + vec->size);
	return (vec);
}

void	tok_array_print(t_token_array *array)
{
	size_t	j;
	t_token	*tok;

	j = 0;
	printf("============================size : %zu ========\n", array->size);
	while (j < array->size)
	{
		tok = &array->items[j];
		if (tok->type == TOK_EOF)
			break ;
		printf("tok->type = %s\n", get_type_as_cstr(tok->type));
		str_print(tok->lexeme);
		j++;
	}
	printf("==============================================\n");
}

void	toks_destroy(t_token_array	*vec)
{
	size_t	i;

	i = 0;
	if (vec)
	{
		if (vec->items)
		{
			while (i < vec->size)
			{
				str_destruct(vec->items[i].lexeme);
				vec->items[i++].lexeme = NULL;
			}
			ft_free(vec->items);
		}
		ft_free(vec);
	}
}
