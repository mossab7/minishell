/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_man_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:20:31 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 20:20:32 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	free_resource(void (*free_func)(void *), void *resource)
{
	if (free_func && resource)
	{
		free_func(resource);
	}
}

void	cleanup_memory_tracker(t_alloc_record **memory_records)
{
	t_alloc_record	*current;
	t_alloc_record	*next;

	if (!memory_records || !*memory_records)
		return ;
	current = *memory_records;
	while (current != NULL)
	{
		next = current->next;
		free_resource(current->free_func, current->resource);
		free(current);
		current = next;
	}
	*memory_records = NULL;
}

void	detach_resource(t_alloc_record **memory_records, void *target_resource)
{
	t_alloc_record	*current;
	t_alloc_record	*temp;

	if (!memory_records || !*memory_records || !target_resource)
		return ;
	current = *memory_records;
	if (current->resource == target_resource)
	{
		*memory_records = current->next;
		free_resource(current->free_func, current->resource);
		free(current);
		return ;
	}
	while (current->next && current->next->resource != target_resource)
		current = current->next;
	if (!current->next)
		return ;
	temp = current->next;
	current->next = temp->next;
	free_resource(temp->free_func, temp->resource);
	free(temp);
}

void	ft_close(void *fd)
{
	int	descriptor;

	if (fd)
	{
		descriptor = (intptr_t)fd;
		close(descriptor);
	}
}
