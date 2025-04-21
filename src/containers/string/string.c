/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:40:18 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/15 15:49:00 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	str_expand_anyhow(t_string *vec)
{
	vec->cap *= VEC_GROWTH_FAC;
	vec->cstring = ft_realloc(vec->cstring, vec->cap, vec->size);
	mask_expand_anyhow(vec->mask);
}

void	str_expand(t_string *vec)
{
	if (vec->size + 1 >= vec->cap)
		str_expand_anyhow(vec);
}

void	str_push_back(t_string *vec, uintptr_t item)
{
	str_expand(vec);
	vec->cstring[vec->size++] = item;
	vec->cstring[vec->size] = 0;
}

t_string	*str_construct(void)
{
	t_string	*vec;

	vec = alloc(sizeof(*vec));
	vec->cstring = alloc(VEC_INIT_CAP);
	vec->cap = VEC_INIT_CAP;
	vec->mask = mask_construct();
	vec->size = 0;
	vec->cursor = 0;
	return (vec);
}

void	str_destruct(t_string *vec)
{
	if (vec)
	{
		if (vec->cstring)
			ft_free(vec->cstring);
		vec->cstring = NULL;
		if (vec->mask)
			mask_destroy(vec->mask);
		vec->mask = NULL;
		ft_free(vec);
	}
}

t_string	*string_dup(t_string *other)
{
	t_string	*new;
	size_t		index;

	new = vstr_construct(1, other->cstring);
	new->mask = mask_construct();
	index = 0;
	while (index < new->mask->size)
	{
		mask_push_back(new->mask,
			other->mask->items[index]);
		index++;
	}
	new->mask->context = other->mask->context;
	return (new);
}
