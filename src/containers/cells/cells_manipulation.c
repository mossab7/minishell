/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cells_manipulation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:47:50 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/23 17:02:17 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	cells_push_back(t_cells *cells, char *key, char *value)
{
	size_t	index;

	index = cells_search(cells, key);
	if (index < cells->size)
	{
		if (cells->items[index].value)
			ft_free(cells->items[index].value);
		cells->items[index].value = ft_strdup(value);
		return ;
	}
	cells_expand(cells);
	cells->items[cells->size].key = ft_strdup(key);
	if (value == NULL)
		cells->items[cells->size].value = NULL;
	else
		cells->items[cells->size].value = ft_strdup(value);
	cells->size++;
}

size_t	cells_search(t_cells *cells, char *key)
{
	size_t	index;

	index = 0;
	if (cells->size == 0)
		return (1);
	while (index < cells->size)
	{
		if (ft_strcmp(cells->items[index].key, key) == 0)
			return (index);
		index++;
	}
	return (index);
}

void	cells_sort(t_cells *cells)
{
	t_cell	tmp;
	size_t	i;
	size_t	j;

	i = 0;
	while (i < cells->size)
	{
		j = i + 1;
		while (j < cells->size)
		{
			if (ft_strcmp(cells->items[i].key, cells->items[j].key) > 0)
			{
				tmp = cells->items[i];
				cells->items[i] = cells->items[j];
				cells->items[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	cells_key_exists(t_cells *cells, char *key)
{
	return (cells_search(cells, key) < cells->size);
}

void	cells_remove(t_cells *cells, char *key)
{
	size_t	index;

	index = cells_search(cells, key);
	if (index < cells->size)
	{
		ft_free(cells->items[index].value);
		ft_free(cells->items[index].key);
		memmove(cells->items + index,
			cells->items + index + 1,
			(cells->size - index - 1) * sizeof(cells->items[0]));
		cells->size--;
	}
}
