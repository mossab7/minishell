/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cells.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:39:58 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/23 17:02:13 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	cells_expand(t_cells *cells)
{
	if(cells->size >= cells->cap)
	{
		cells->cap *= VEC_GROWTH_FAC;
		cells->items = ft_realloc(cells->items,
					cells->cap * sizeof(*(cells->items)),
					cells->size * sizeof(*(cells->items)));
	}
}

t_cells	*cells_construct(void)
{
	t_cells	*cells;

	cells = alloc(sizeof(*cells));
	cells->items = alloc(VAC_INIT_CAP * sizeof(*(cells->items)));
	cells->cap = VEC_INIT_CAP;
	cells->size = 0;
	return (cells);
}

void	cells_destroy(t_cells *cells)
{
	size_t	index;

	index = 0;
	if (cells)
	{
		while (index < cells->size)
		{
			ft_free(cells->items[index].key);
			ft_free(cells->items[index].value);
			index++;
		}
		if (cells->items)
			ft_free(cells->items);
		ft_free(cells);
	}
}
