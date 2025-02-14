#ifndef T_STING_H
# define T_STING_H
#include <stdlib.h>
#include <stdint.h>

#define VAC_INIT_CAP 16
#define VAC_GROWTH_FACTOR 2

typedef struct s_string
{
	char	*cstring;
	size_t	size;
	size_t	cap;
}	t_string;

t_string	*str_construct(void);
t_string	*vstr_construct(size_t count, ...);
void		str_expand(t_string *vec);
void		str_push_back(t_string *vec, uintptr_t item);
void		str_append(char *src,t_string *vec);
void		str_overwrite(char *src,t_string *vec);
void		str_destruct(t_string *vec);
void		str_join(t_string	*vec, size_t count, ...);
#endif // !T_STING_H
