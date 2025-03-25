/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cells.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:39:58 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 16:15:52 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	cells_expand(t_cells *cells)
{
	if (cells->size >= cells->cap)
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

void	insert_if_not_found(t_cells *cells, char *key, char *value)
{
	size_t	index;

	index = cells_search(cells, key);
	if (index < cells->size)
		return ;
	cells_expand(cells);
	cells->items[cells->size].key = ft_strdup(key);
	if (value == NULL)
		cells->items[cells->size].value = NULL;
	else
		cells->items[cells->size].value = ft_strdup(value);
	cells->size++;
}
