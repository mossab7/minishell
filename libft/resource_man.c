/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_man.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:17:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 20:17:42 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>

t_alloc_record	**get_memory_tracker(void)
{
	static t_alloc_record	*memory_records;

	return (&memory_records);
}

t_alloc_record	*create_memory_record(void *resource,
		void (*deallocator)(void *))
{
	t_alloc_record	*new_record;

	new_record = malloc(sizeof(t_alloc_record));
	if (!new_record)
		return (NULL);
	new_record->resource = resource;
	new_record->free_func = deallocator;
	new_record->next = NULL;
	return (new_record);
}

void	register_memory_allocation(t_alloc_record **memory_records,
		t_alloc_record *new_record)
{
	t_alloc_record	*current;

	if (!new_record)
		return ;
	if (!*memory_records)
	{
		*memory_records = new_record;
		return ;
	}
	current = *memory_records;
	while (current->next)
		current = current->next;
	current->next = new_record;
}

void	handle_allocation_failure(void *resource)
{
	if (resource == NULL)
	{
		cleanup_memory_tracker(get_memory_tracker());
		ft_putstr_fd("Fatal: Memory allocation failed\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

void	*allocate_tracked_memory(size_t size)
{
	void	*resource;

	resource = ft_calloc(size, 1);
	handle_allocation_failure(resource);
	register_memory_allocation(get_memory_tracker(),
		create_memory_record(resource, free));
	return (resource);
}
