/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_mask.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:18:13 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/28 15:17:14 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	mask_expand_anyhow(t_mask *mask)
{
	mask->cap *= VEC_GROWTH_FAC;
	mask->items = ft_realloc(mask->items,
			mask->cap * sizeof(mask->items[0]),
			mask->size * sizeof(mask->items[0]));
	mask->ids = ft_realloc(mask->ids,
			mask->cap * sizeof(mask->ids[0]),
			mask->size * sizeof(mask->ids[0]));
}

void	mask_expand(t_mask *mask)
{
	if (mask->size + 1 >= mask->cap)
		mask_expand_anyhow(mask);
}

void	mask_push_back(t_mask *mask, t_u8 item, unsigned int id)
{
	mask_expand(mask);
	mask->items[mask->size] = item;
	mask->ids[mask->size++] = id;
	mask->items[mask->size] = 0;
	mask->ids[mask->size] = 0;
}

t_mask	*mask_construct(void)
{
	t_mask	*mask;

	mask = alloc(sizeof(*mask));
	mask->items = alloc(VAC_INIT_CAP * sizeof(*mask->items));
	mask->ids = alloc(VAC_INIT_CAP * sizeof(*mask->ids));
	mask->cap = VEC_INIT_CAP;
	mask->size = 0;
	mask->cursor = 0;
	return (mask);
}

void	mask_destroy(t_mask *mask)
{
	if (mask)
	{
		if (mask->items)
			ft_free(mask->items);
		if (mask->ids)
			ft_free(mask->ids);
		ft_free(mask);
	}
}
