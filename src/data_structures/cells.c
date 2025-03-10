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

void	cells_push_back(t_cells *cells, char *key, char *value)
{
	cells_expand(cells);
	cells->items[cells->size].key = ft_strdup(key);
	if (value == NULL)
		cells->items[cells->size].value = NULL;
	else
		cells->items[cells->size].value = ft_strdup(value);
	cells->size++;
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

size_t	cells_search(t_cells *cells, char *key)
{
	size_t	index;

	index = 0;
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
	size_t		i;
	size_t		j;

	i = 0;
	while (i < cells->size)
	{
		j = i + 1;
		while (j < cells->size)
		{
			// NOTE: Zakaria's contribution
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
