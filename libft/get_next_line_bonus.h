/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:13:27 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/05 16:29:26 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>
# include <libft.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 512
# endif // !BUFFER_SIZE
# define FD_MAX 10240

typedef struct s_line
{
	size_t	cap;
	size_t	size;
	char	*content;
}			t_line;

ssize_t		line_read(int fd, t_line *line, char **nl_loc, ssize_t *nread);
char		*get_next_line(int fd);
void		*ft_realloc_gnl(void *ptr, size_t new_sz, size_t old_sz);
char		*ft_strchr_gnl(char *s, int c);
char		*ft_strdup_heap(const char *s1, char *heap);
char		*ft_strcpy_until(char *dst, const char *src, char c);

#endif // !GET_NEXT_LINE_BONUS_H
