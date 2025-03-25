/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:31:44 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/02 11:31:47 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <libft.h>

char	*ft_strdup_heap(const char *s1, char *heap)
{
	char	*dup;
	size_t	size;

	if (!s1)
		return (NULL);
	size = 0;
	while (s1[size])
		size++;
	dup = ft_realloc_gnl(NULL, size + 1, 0);
	if (!dup)
	{
		if (heap)
			ft_free(heap);
		return (NULL);
	}
	dup = ft_strcpy_until(dup, s1, 0);
	if (heap)
		ft_free(heap);
	return (dup);
}

void	*ft_realloc_gnl(void *ptr, size_t new_sz, size_t old_sz)
{
	char	*new;
	size_t	index;

	new = alloc(new_sz);
	if (new == NULL)
	{
		ft_free(ptr);
		return (NULL);
	}
	index = 0;
	while (index < new_sz)
		new[index++] = 0;
	if (!ptr)
		return (new);
	index = 0;
	while (index < old_sz)
	{
		new[index] = ((char *)ptr)[index];
		index++;
	}
	ft_free(ptr);
	return (new);
}

char	*ft_strchr_gnl(char *s, int c)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] != c)
	{
		if (!s[i])
			break ;
		i++;
	}
	if (s[i] == c)
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_strcpy_until(char *dst, const char *src, char c)
{
	size_t	index;
	size_t	size;

	if (!dst && !src)
		return (NULL);
	index = 0;
	size = 0;
	while (src[size])
		size++;
	while (index < size)
	{
		dst[index] = src[index];
		if (dst[index] == c)
			break ;
		index++;
	}
	return (dst);
}

ssize_t	line_read(int fd, t_line_gnl *line, char **nl_loc, ssize_t *nread)
{
	if (line->size + BUFFER_SIZE >= line->cap)
	{
		line->content = ft_realloc_gnl(line->content, (line->cap * 2),
				line->size);
		if (!line->content)
			return (-1);
		line->cap *= 2;
	}
	*nread = (read(fd, (line->content + line->size), BUFFER_SIZE));
	if (*nread <= 0)
		return (*nread);
	*nl_loc = ft_strchr_gnl(line->content + line->size, '\n');
	line->size += *nread;
	return (*nread);
}
