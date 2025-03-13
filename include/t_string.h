#ifndef T_STING_H
# define T_STING_H
#include <stdlib.h>
#include <zenint.h>

#define VAC_INIT_CAP 16
#define VAC_GROWTH_FACTOR 2

typedef struct s_mask {
	u8		context;
	u8		*items;
	size_t	cap;
	size_t	size;
}	t_mask;

typedef struct s_string
{
	t_mask	*mask;
	char	*cstring;
	size_t	size;
	size_t	cap;
}	t_string;

t_string	*str_construct(void);
int			str_search(t_string *vec, const char *text);
t_string	*vstr_construct(size_t count, ...);
void		str_expand(t_string *vec);
void		str_push_back(t_string *vec, uintptr_t item);
void		str_append(char *src,t_string *vec);
void		str_overwrite(char *src,t_string *vec);
void		str_destruct(t_string *vec);
void		str_join(t_string	*vec, size_t count, ...);
void		str_substitute(t_string *vec, char *repl, char *which);
void		mask_expand(t_mask *mask);
void		mask_push_back(t_mask *mask, u8 item);
t_mask		*mask_construct(void);
void		mask_destroy(t_mask *mask);
void		mask_expand_anyhow(t_mask *mask);
void		str_print(t_string *str);
#endif // !T_STING_H
