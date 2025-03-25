/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_methods.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:41:14 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/18 16:40:32 by lazmoud          ###   ########.fr       */
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

static void	process_env_entries(t_env *env, const char *envp[],
		int *shell_level_index, u8 *pwd_set)
{
	size_t	iter;
	char	**entry;

	iter = 0;
	while (envp[iter])
	{
		entry = ft_split((char const *)envp[iter], '=');
		env_append_both(env, entry[KEY_INDEX], entry[VALUE_INDEX]);
		if (ft_strcmp(entry[KEY_INDEX], "PATH") == 0)
			parse_path(env->path, entry[VALUE_INDEX]);
		if (ft_strcmp(entry[KEY_INDEX], "SHLVL") == 0)
			*shell_level_index = iter;
		if (ft_strcmp(entry[KEY_INDEX], "PWD") == 0)
			*pwd_set = 1;
		ft_free(entry[KEY_INDEX]);
		ft_free(entry[VALUE_INDEX]);
		ft_free(entry);
		iter++;
	}
	env_join(env);
}

static void	set_default_env_values(t_env *env, int shell_level_index,
		u8 pwd_set)
{
	char	pwd[PATH_MAX];

	if (shell_level_index == -1)
		env_append_both(env, "SHLVL", "1");
	else
		increment_shell_level(env, shell_level_index);
	if (!pwd_set)
	{
		getcwd(pwd, PATH_MAX);
		env_append_both(env, "PWD", pwd);
	}
}

t_env	*env_parse(const char *envp[])
{
	int		shell_level_index;
	u8		pwd_set;
	t_env	*env;

	env = env_construct();
	shell_level_index = -1;
	pwd_set = 0;
	process_env_entries(env, envp, &shell_level_index, &pwd_set);
	set_default_env_values(env, shell_level_index, pwd_set);
	return (env);
}

static void	free_env_pointers(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		ft_free(envp[i]);
		i++;
	}
	ft_free(envp);
}

static char	**create_env_pointers(t_env *env)
{
	char	**envp;
	char	*entry;
	t_cell	cell;
	size_t	iter;

	envp = alloc((env->cells->size + 1) * sizeof(*envp));
	iter = 0;
	while (iter < env->cells->size)
	{
		cell = env->cells->items[iter];
		entry = ft_strjoin(cell.key, "=");
		envp[iter++] = ft_strjoin(entry, cell.value);
		ft_free(entry);
	}
	envp[iter] = NULL;
	return (envp);
}

void	env_join(t_env *env)
{
	if (!env || !(env->cells) || !env->cells->size)
		return ;
	free_env_pointers(env->envp);
	env->envp = create_env_pointers(env);
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
		ft_memmove(env->cells->items + index, env->cells->items + index + 1,
			env->cells->size - index);
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
