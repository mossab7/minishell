#include <libft.h>
#include <stdio.h>

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

// <File 2>
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

/*
** This function is used to close a file descriptor.
** how to use it:
** int fd = open("test.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
** register_memory_allocation(get_memory_tracker(),
	create_memory_record((void *)(intptr_t)fd, ft_close));
*/

void	ft_close(void *fd)
{
	int	descriptor;

	if (fd)
	{
		descriptor = (intptr_t)fd;
		close(descriptor);
	}
}
