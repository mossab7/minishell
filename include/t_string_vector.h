/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_string_vector.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/24 19:43:41 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_STRING_VECTOR_H
# define T_STRING_VECTOR_H
# include <stdint.h>
# include <stdlib.h>

# define VAC_INIT_CAP 16
# define VAC_GROWTH_FACTOR 2

typedef struct s_string_vector
{
	char		**cstrings;
	size_t		size;
	size_t		cap;
}				t_string_vector;

void			strv_expand(t_string_vector *vec);
void			strv_push_back(t_string_vector *vec, char *item);
void			strv_append(char *src, t_string_vector *vec);
t_string_vector	*strv_construct(void);
void			strv_destruct(t_string_vector *vec);
#endif // !T_STRING_VECTOR_H
