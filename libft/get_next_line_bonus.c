/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:08:37 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/05 16:54:28 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_bonus.h"

static void	*release_gnl_resources(t_line *line, char *table[FD_MAX], int fd)
{
	if (fd >= 0)
	{
		if (table[fd])
		{
			free(table[fd]);
			table[fd] = NULL;
		}
	}
	if (!line)
		return (NULL);
	if (!line->content)
		return (NULL);
	if (line->size == 0)
	{
		free(line->content);
		line->content = NULL;
	}
	return (NULL);
}

static int	stash_line(t_line *line, char *table[FD_MAX], int fd)
{
	char	*nl_location;
	int		ln;

	if (!table[fd] || !line->content)
		return (0);
	line->content = ft_strcpy_until(line->content, table[fd], '\n');
	nl_location = ft_strchr(table[fd], '\n');
	if (nl_location)
	{
		if (*(nl_location + 1))
		{
			table[fd] = ft_strdup(nl_location + 1, table[fd]);
			if (table[fd] == NULL)
				release_gnl_resources(line, NULL, -1);
			return (-1);
		}
		release_gnl_resources(NULL, table, fd);
		return (-1);
	}
	release_gnl_resources(NULL, table, fd);
	ln = 0;
	while (line->content && line->content[ln])
		ln++;
	return (ln);
}

static int	read_next_chunk(int fd, t_line *line, char **rest)
{
	ssize_t	nread;
	char	*nl_location;

	while (1)
	{
		if (line_read(fd, line, &nl_location, &nread) <= 0)
			return (nread);
		if (nl_location)
		{
			if (*(nl_location + 1))
			{
				*rest = ft_strdup(nl_location + 1, *rest);
				*(nl_location + 1) = 0;
				if (*rest == NULL)
				{
					line->size = 0;
					release_gnl_resources(line, NULL, -1);
					return (-1);
				}
				return (1);
			}
			return (0);
		}
	}
	return (1);
}

static char	*string_create(t_line *line, char *table[FD_MAX], int fd)
{
	char	*buff;

	if (line->content == NULL)
		return (NULL);
	buff = ft_strdup(line->content, NULL);
	if (buff == NULL)
	{
		line->size = 0;
		return (release_gnl_resources(line, table, fd));
	}
	line->size = 0;
	release_gnl_resources(line, NULL, -1);
	return (buff);
}

char	*get_next_line(int fd)
{
	static char	*rest[FD_MAX];
	int			nstashed;
	t_line		line;
	size_t		sz;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (release_gnl_resources(NULL, rest, fd));
	sz = 0;
	while (rest[fd] && rest[fd][sz])
		sz++;
	line.cap = BUFFER_SIZE + sz + 1;
	line.size = 0;
	line.content = ft_realloc(NULL, line.cap, line.size);
	if (!line.content)
		return (release_gnl_resources(&line, rest, fd));
	nstashed = stash_line(&line, rest, fd);
	if (((read(fd, 0, 0) < 0) && nstashed != 0) || (nstashed == -1))
		return (string_create(&line, rest, fd));
	line.size = nstashed;
	if (read_next_chunk(fd, &line, rest + fd) <= 0)
		release_gnl_resources(&line, rest, fd);
	return (string_create(&line, rest, fd));
}
