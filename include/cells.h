#ifndef CELLS_H

typedef struct s_cell
{
	char	*key;
	char	*value;
}	t_cell;

typedef struct s_cells
{
	t_cell	*items;
	size_t	size;
	size_t	cap;
}	t_cells;

size_t	cells_search(t_cells *cells, char *key);
void	cells_destroy(t_cells *cells);
t_cells	*cells_construct(void);
void	cells_push_back(t_cells *cells, char *key, char *value);
void	cells_expand(t_cells *cells);
void	cells_sort(t_cells *cells);
#endif // !CELLS_H
