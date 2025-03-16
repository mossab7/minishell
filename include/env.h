#ifndef ENV_H
# define ENV_H

# define KEY_INDEX 0
# define VALUE_INDEX 1
# include <t_string_vector.h>
#include <cells.h>
#include <zenint.h>

typedef struct s_env
{
	t_cells			*cells;
	t_cells			*export_cells;
	char			**envp;
	t_string_vector	*path;
	int				last_command_status;
}	t_env;

t_env		*env_parse(const char *envp[]);
void		env_join(t_env *env);
void		env_append_both(t_env *env, char *key, char *value);
void		env_del(t_env *env, char *key);
void		env_destroy(t_env *env);
void		env_expand(t_env *env);
t_env		*env_construct();
size_t		env_search(t_env *env, char *key);
t_env		*env_copy(t_env *env);
void		env_set(t_env *env, char *key, char *new_value);
char		*env_get(t_env *env, char *key);
void		env_print(t_env *env);
void		parse_path(t_string_vector *path, char *src);
t_string	*search_path(t_string_vector *path, char *command);
int			cells_key_exists(t_cells *cells, char *key);
#endif // !ENV_H
