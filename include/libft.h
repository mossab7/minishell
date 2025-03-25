/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 20:11:07 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 17:01:35 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIBFT_H
# define LIBFT_H
# include <readline/history.h>
# include <readline/readline.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <t_string.h>
# include <unistd.h>

# define MAX_LONG 9223372036854775807
# define NON -2
# define OVER_FLOW_DETECTED -1
# define SUCCESS 1
# define DONE 0

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif // !BUFFER_SIZE
# define FD_MAX 10240

typedef struct s_num
{
	int						sign;
	const char				*base;
	long					*number;
}							t_num;

typedef struct s_list
{
	void					*content;
	struct s_list			*next;
}							t_list;

typedef struct s_alloc_record
{
	void					*resource;
	void					(*free_func)(void *);
	struct s_alloc_record	*next;
}							t_alloc_record;

typedef struct s_line_gnl
{
	size_t					cap;
	size_t					size;
	char					*content;
}							t_line_gnl;

int					ft_atoi(const char *str);
int					ft_atol_base(const char *str, const char *base,
						long *where);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t count, size_t size);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
int					ft_isspace(int c);
int					ft_ispunct(int c);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memcpy(void *restrict dst,
						const void *restrict src, size_t n);
void				*ft_memmove(void *dst, const void *src, size_t len);
void				*ft_memset(void *b, int c, size_t len);
char				*ft_strdup(const char *s1);
size_t				ft_strlcpy(char *restrict dst,
						const char *restrict src, size_t dstsize);
size_t				ft_strlcat(char *restrict dst,
						const char *restrict src, size_t dstsize);
size_t				ft_strlen(char *s);
char				*ft_strnstr(const char *haystack,
						const char *needle, size_t len);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strrchr(const char *s, int c);
int					ft_tolower(int c);
int					ft_toupper(int c);
int					ft_strncmp(const char *s1, const char *s2,
						size_t n);
char				*ft_strnstr(const char *haystack,
						const char *needle, size_t len);
char				*ft_strrchr(const char *s, int c);
char				*ft_substr(char const *s, unsigned int start,
						size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int,
							char));
void				ft_striteri(char *s, void (*f)(unsigned int,
							char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
void				*allocate_tracked_memory(size_t size);
void				handle_allocation_failure(void *resource);
void				register_memory_allocation(
						t_alloc_record **memory_records,
						t_alloc_record *new_record);
t_alloc_record		*create_memory_record(void *resource,
						void (*deallocator)(void *));
t_alloc_record		**get_memory_tracker(void);
void				detach_resource(t_alloc_record **memory_records,
						void *target_resource);
void				cleanup_memory_tracker(
						t_alloc_record **memory_records);
void				free_resource(void (*free_func)(void *),
						void *resource);
void				*ft_realloc(void *ptr, size_t new_sz,
						size_t old_sz);
void				ft_free(void *addr);
void				*alloc(size_t cap);
void				ft_close(void *fd);
int					is_quote(int c);
t_string			*ft_readline(const char *prompt);
ssize_t				line_read(int fd, t_line_gnl *line, char **nl_loc,
						ssize_t *nread);
char				*get_next_line(int fd);
void				*ft_realloc_gnl(void *ptr, size_t new_sz,
						size_t old_sz);
char				*ft_strchr_gnl(char *s, int c);
char				*ft_strdup_heap(const char *s1, char *heap);
char				*ft_strcpy_until(char *dst, const char *src,
						char c);
char				*ft_strchr(char *s, int c);
int					is_not_null(int c);
#endif // LIBFT_H
