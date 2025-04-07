/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_string.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:15:46 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 19:43:41 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef T_STRING_H
# define T_STRING_H
# include <stdlib.h>
# include <zenint.h>

# define VAC_INIT_CAP 16
# define VAC_GROWTH_FACTOR 2

typedef struct s_mask
{
	t_u8	context;
	t_u8	*items;
	size_t	cap;
	size_t	size;
	size_t	cursor;
}			t_mask;

typedef struct s_string
{
	t_mask	*mask;
	char	*cstring;
	size_t	size;
	size_t	cap;
	int		cursor;
}			t_string;

void		str_shift_left(t_string *str, int new_dest, int size);
void		str_shift_right(t_string *string, int old_dest, int size);
t_string	*str_construct(void);
int			str_search(t_string *vec, const char *text, int offset);
int			str_search_using_cursor(t_string *vec, const char *text);
t_string	*vstr_construct(size_t count, ...);
void		str_expand(t_string *vec);
void		str_push_back(t_string *vec, uintptr_t item);
void		str_append(char *src, t_string *vec);
void		str_overwrite(char *src, t_string *vec);
void		str_destruct(t_string *vec);
void		str_join(t_string *vec, size_t count, ...);
void		str_substitute(t_string *string, char *repl, t_string *which);
void		mask_expand(t_mask *mask);
void		mask_push_back(t_mask *mask, t_u8 item);
t_mask		*mask_construct(void);
void		mask_destroy(t_mask *mask);
void		mask_expand_anyhow(t_mask *mask);
void		str_expand_anyhow(t_string *vec);
void		str_print(t_string *str);
void		mask_fill(t_mask *mask, t_u8 item, size_t size);
void		mask_copy(t_mask *dst, t_mask *src, size_t start);
void		mask_print(t_mask *mask);
void		mask_copy_ignore_spaces(t_mask *dst, t_string *src, size_t start);
int			is_dollar_sign_not_expandable(t_string *string, int index);
#endif // !T_STRING_H
