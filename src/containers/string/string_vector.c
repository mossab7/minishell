/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_vector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:53:21 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/23 17:18:01 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	strv_expand(t_string_vector *vec)
{
	if(vec->size + 1 >= vec->cap)
	{
		vec->cap *= VEC_GROWTH_FAC;
		vec->cstrings = ft_realloc(vec->cstrings,
					vec->cap * sizeof(*vec->cstrings), vec->size * sizeof(*vec->cstrings));
	}
}

void	strv_push_back(t_string_vector *vec, char *item)
{
	if (!item)
		return ;
	strv_expand(vec);
	vec->cstrings[vec->size++] = ft_strdup(item);
	vec->cstrings[vec->size] = NULL;
}

void	strv_destruct(t_string_vector *vec)
{
	size_t	index;

	index = 0;
	while (index < vec->size)
		ft_free(vec->cstrings[index++]);
	ft_free(vec->cstrings);
	ft_free(vec);
}

t_string_vector	*strv_construct()
{
	t_string_vector	*vec;

	vec = alloc(sizeof(*vec));
	vec->cstrings = alloc(VEC_INIT_CAP * sizeof(*vec->cstrings));
	vec->cap = VEC_INIT_CAP;
	vec->size = 0;
	return (vec);
}
