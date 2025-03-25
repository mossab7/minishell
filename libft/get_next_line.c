/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:31:23 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/02 12:38:30 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <libft.h>

static void	*release_gnl_resources(t_line_gnl *line, char **rest)
{
	if (rest)
	{
		ft_free(*rest);
		*rest = NULL;
	}
	if (!line)
		return (NULL);
	if (!line->content)
		return (NULL);
	if (line->size == 0)
	{
		ft_free(line->content);
		line->content = NULL;
	}
	return (NULL);
}

static int	stash_line(char **line, char **rest)
{
	char	*nl_location;
	int		ln;

	if (!*rest || !*line)
		return (0);
	*line = ft_strcpy_until(*line, *rest, '\n');
	nl_location = ft_strchr_gnl(*rest, '\n');
	if (nl_location)
	{
		if (*(nl_location + 1))
		{
			*rest = ft_strdup_heap(nl_location + 1, *rest);
			if (rest == NULL)
				ft_free(*line);
			return (-1);
		}
		release_gnl_resources(NULL, rest);
		return (-1);
	}
	release_gnl_resources(NULL, rest);
	ln = 0;
	while (*line && *(*line + ln))
		ln++;
	return (ln);
}

static int	read_next_chunk(int fd, t_line_gnl *line, char **rest)
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
				*rest = ft_strdup_heap(nl_location + 1, *rest);
				*(nl_location + 1) = 0;
				if (*rest == NULL)
				{
					line->size = 0;
					release_gnl_resources(line, NULL);
					return (-1);
				}
				return (1);
			}
			return (0);
		}
	}
	return (1);
}

static char	*string_create(t_line_gnl *line, char **rest)
{
	char	*buff;

	if (line->content == NULL)
		return (NULL);
	buff = ft_strdup_heap(line->content, NULL);
	if (buff == NULL)
	{
		line->size = 0;
		return (release_gnl_resources(line, rest));
	}
	line->size = 0;
	release_gnl_resources(line, NULL);
	return (buff);
}

char	*get_next_line(int fd)
{
	static char	*rest;
	int			nstashed;
	t_line_gnl	line;
	size_t		sz;

	sz = 0;
	while (rest && rest[sz])
		sz++;
	line.cap = BUFFER_SIZE + sz + 1;
	line.size = 0;
	line.content = ft_realloc_gnl(NULL, line.cap, line.size);
	if (fd < 0 || BUFFER_SIZE <= 0 || (read(fd, 0, 0) < 0) || !line.content)
		return (release_gnl_resources(&line, &rest));
	nstashed = stash_line(&(line.content), &rest);
	if (((read(fd, 0, 0) < 0) && nstashed != 0) || (nstashed == -1))
		return (string_create(&line, &rest));
	line.size = nstashed;
	if (read_next_chunk(fd, &line, &rest) <= 0)
		release_gnl_resources(&line, &rest);
	return (string_create(&line, &rest));
}
