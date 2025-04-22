/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_dup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:01:14 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/22 11:01:27 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

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
