/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_vector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 20:53:21 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 15:51:52 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	strv_expand(t_string_vector *vec)
{
	if (vec->size + 1 >= vec->cap)
	{
		vec->cap *= VEC_GROWTH_FAC;
		vec->cstrings = ft_realloc(vec->cstrings,
				vec->cap * sizeof(*vec->cstrings),
				vec->size * sizeof(*vec->cstrings));
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

t_string_vector	*strv_construct(void)
{
	t_string_vector	*vec;

	vec = alloc(sizeof(*vec));
	vec->cstrings = alloc(VEC_INIT_CAP * sizeof(*vec->cstrings));
	vec->cap = VEC_INIT_CAP;
	vec->size = 0;
	return (vec);
}

void	strv_sort(t_string_vector *vector)
{
	char	*tmp;
	size_t	i;
	size_t	j;

	i = 0;
	while (i < vector->size)
	{
		j = i + 1;
		while (j < vector->size)
		{
			if (compare_alpha(vector->cstrings[i], vector->cstrings[j]) > 0)
			{
				tmp = vector->cstrings[i];
				vector->cstrings[i] = vector->cstrings[j];
				vector->cstrings[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
