/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_methods.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:41:14 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/17 16:56:36 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	increment_shell_level(t_env *env, int index)
{
	long	lvl;
	int		res;

	if (index < 0)
		return ;
	res = ft_atol_base(env->cells->items[index].value, BASE_10, &lvl);
	lvl++;
	if (res == NON || res == OVER_FLOW_DETECTED || lvl > SHLVL_MAX)
	{
		zen_elog("warning: shell level (%d) too high, resetting to 1\n", lvl);
		lvl = 1;
	}
	if (lvl < 0)
		lvl = 0;
	ft_free(env->cells->items[index].value);
	env->cells->items[index].value = ft_itoa(lvl);
}

// Today's run:
// If these were not parsed they should exist.
// [*] - PWD
// [*] - SHLVL
// [*] - _
// [*] - OLDPWD Empty in export_env
t_env	*env_parse(const char *envp[])
{
	size_t	iter;
	char	pwd[PATH_MAX];
	u8		pwd_set;
	int		shell_level_index;
	char **entry;
	t_env	*env;

	iter = 0;
	env = env_construct();
	shell_level_index = -1;
	pwd_set = 0;
	while (envp[iter])
	{
		// LEAK: entry leaks, free it
		entry = ft_split((char const *)envp[iter], '=');
		env_append_both(env, entry[KEY_INDEX], entry[VALUE_INDEX]);
		if (ft_strcmp(entry[KEY_INDEX], "PATH") == 0)
			parse_path(env->path, entry[VALUE_INDEX]);
		if (ft_strcmp(entry[KEY_INDEX], "SHLVL") == 0)
			shell_level_index = iter;
		if (ft_strcmp(entry[KEY_INDEX], "PWD") == 0)
			pwd_set = 1;
		ft_free(entry[KEY_INDEX]);
		ft_free(entry[VALUE_INDEX]);
		ft_free(entry);
		iter++;
	}
	env_join(env);
	if (shell_level_index == -1)
		env_append_both(env, "SHLVL", "1");
	else
		increment_shell_level(env, shell_level_index);
	if (!pwd_set)
	{
		getcwd(pwd, PATH_MAX);
		env_append_both(env, "PWD", pwd);
	}
	return (env);
}

void	env_join(t_env *env)
{
	char	*entry;
	t_cell	cell;
	size_t	iter;

	if (!env || !(env->cells) || !env->cells->size)
		return ;
	if (env->envp)
	{
		size_t i = 0;
		while(env->envp[i])
		{
			ft_free(env->envp[i]);
			i++;
		}
		ft_free(env->envp);
	}
	iter = 0;
	env->envp = alloc((env->cells->size + 1 ) * sizeof(*env->envp));
	while (iter < env->cells->size)
	{
		cell = env->cells->items[iter];
		entry = ft_strjoin(cell.key, "=");
		env->envp[iter++] = ft_strjoin(entry, cell.value);
		ft_free(entry);
	}
	env->envp[iter] = NULL;
}

void	env_append_both(t_env *env, char *key, char *value)
{
	cells_push_back(env->cells, key, value);
	cells_push_back(env->export_cells, key, value);
}

void	env_del(t_env *env, char *key)
{
	size_t	index;

	index = env_search(env, key);
	if (index < env->cells->size)
	{
		ft_memmove(env->cells->items + index, env->cells->items + index + 1, env->cells->size - index);
		env->cells->size--;
	}
}

void	env_set(t_env *env, char *key, char *new_value)
{
	if (ft_strcmp(key, "PATH") == 0)
		parse_path(env->path, new_value);
	env_append_both(env, key, new_value);
	env_join(env);
}

char *env_get(t_env *env, char *key)
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
