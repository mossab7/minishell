#ifndef T_STRING_H
# define T_STRING_H
#include <stdlib.h>
#include <stdint.h>

#define VAC_INIT_CAP 16
#define VAC_GROWTH_FACTOR 2

typedef struct s_string_vector
{
	char	**cstrings;
	size_t	size;
	size_t	cap;
}	t_string_vector;

void			strv_expand(t_string_vector *vec);
void			strv_push_back(t_string_vector *vec, char *item);
void			strv_append(char *src,t_string_vector *vec);
t_string_vector	*strv_construct();
#endif // !T_STRING_H
