/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:49:57 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 19:43:41 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif // !BUFFER_SIZE

typedef struct s_line
{
	size_t	cap;
	size_t	size;
	char	*content;
}			t_line;

ssize_t		line_read(int fd, t_line *line, char **nl_loc, ssize_t *nread);
char		*get_next_line(int fd);
void		*ft_realloc(void *ptr, size_t new_sz, size_t old_sz);
char		*ft_strdup_heap(const char *s1, char *heap);
char		*ft_strchr(char *s, int c);
char		*ft_strcpy_until(char *dst, const char *src, char c);

#endif // !GET_NEXT_LINE_H
