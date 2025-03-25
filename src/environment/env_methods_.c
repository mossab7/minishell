/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_methods_.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:56:12 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 16:30:02 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	env_set(t_env *env, char *key, char *new_value)
{
	if (ft_strcmp(key, "PATH") == 0)
		parse_path(env->path, new_value);
	env_append_both(env, key, new_value);
	env_join(env);
}

char	*env_get(t_env *env, char *key)
{
	size_t	index;

	index = env_search(env, key);
	if (index < env->cells->size)
		return (env->cells->items[index].value);
	return ("");
}

void	env_print(t_env *env)
{
	size_t	iter;
	t_cell	cell;

	if (!env || !env->cells || !env->cells->size)
		return ;
	iter = 0;
	while (iter < env->cells->size)
	{
		cell = env->cells->items[iter];
		ft_printf("%s=%s\n", cell.key, cell.value);
		iter++;
	}
}

void	env_append_both(t_env *env, char *key, char *value)
{
	cells_push_back(env->cells, key, value);
	cells_push_back(env->export_cells, key, value);
}
