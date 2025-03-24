/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:41:08 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 06:41:10 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

t_env	*env_construct(void)
{
	t_env	*env;

	env = alloc(sizeof(*env));
	env->cells = cells_construct();
	env->export_cells = cells_construct();
	env->path = strv_construct();
	env->last_command_status = 0;
	return (env);
}

t_env	*env_copy(t_env *env)
{
	t_env	*_env;

	_env = alloc(sizeof(*env));
	_env->cells = cells_construct();
	_env->export_cells = cells_construct();
	_env->path = strv_construct();
	while (_env->cells->size < env->cells->size)
	{
		cells_push_back(_env->cells, env->cells->items[_env->cells->size].key,
			env->cells->items[_env->cells->size].value);
	}
	for (; _env->path->size < env->path->size;)
		strv_push_back(_env->path, env->path->cstrings[_env->path->size]);
	return (_env);
}

void	env_destroy(t_env *env)
{
	if (env)
	{
		cells_destroy(env->cells);
		cells_destroy(env->export_cells);
		strv_destruct(env->path);
		ft_free(env);
	}
}

size_t	env_search(t_env *env, char *key)
{
	return (cells_search(env->cells, key));
}
