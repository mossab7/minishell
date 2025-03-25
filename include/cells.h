/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cells.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 16:16:07 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CELLS_H
# define CELLS_H

typedef struct s_cell
{
	char	*key;
	char	*value;
}			t_cell;

typedef struct s_cells
{
	t_cell	*items;
	size_t	size;
	size_t	cap;
}			t_cells;

size_t	cells_search(t_cells *cells, char *key);
void	cells_destroy(t_cells *cells);
t_cells	*cells_construct(void);
void	cells_push_back(t_cells *cells, char *key, char *value);
void	cells_expand(t_cells *cells);
void	cells_sort(t_cells *cells);
void	cells_remove(t_cells *cells, char *key);
void	insert_if_not_found(t_cells *cells, char *key, char *value);
#endif // !CELLS_H
