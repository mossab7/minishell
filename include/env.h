/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/04/17 04:14:02 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# define KEY_INDEX 0
# define VALUE_INDEX 1
# include <cells.h>
# include <t_string.h>
# include <t_string_vector.h>
# include <zenint.h>

#define COMMAND_NOT_FOUND 127
#define PERM_DENIED 126

typedef struct s_env
{
	t_cells			*cells;
	t_cells			*export_cells;
	char			**envp;
	t_string_vector	*path;
	int				last_command_status;
	const char		*program;
}					t_env;

t_env				*env_parse(const char *envp[], const char *program);
void				env_join(t_env *env);
void				env_append_both(t_env *env, char *key, char *value);
void				env_del(t_env *env, char *key);
void				env_destroy(t_env *env);
void				env_expand(t_env *env);
t_env				*env_construct(void);
size_t				env_search(t_env *env, char *key);
t_env				*env_copy(t_env *env);
void				env_set(t_env *env, char *key, char *new_value);
char				*env_get(t_env *env, char *key);
void				env_print(t_env *env);
void				parse_path(t_string_vector *path, char *src);
t_string			*search_path(t_string_vector *path, char *command, int *code);
int					cells_key_exists(t_cells *cells, char *key);
void				env_set_defaults(t_env *env);
void				increment_shell_level(t_env *env);
#endif // !ENV_H
