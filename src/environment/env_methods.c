#include <zen.h>
#define SHLVL_MAX 1000
void	increment_shell_level(t_env *env, int index)
{
	long	lvl;
	int		res;

	if (index < 0)
		return ;
	res = ft_atol_base(env->cells[index].value, BASE_10, &lvl);
	lvl++;
	if (res == NON || res == OVER_FLOW_DETECTED || lvl < 0 || lvl > SHLVL_MAX)
		lvl = 0;
	ft_free(env->cells[index].value);
	env->cells[index].value = ft_itoa(lvl);
}

t_env	*env_parse(const char *envp[])
{
	size_t	iter;
	int		shell_level_index;
	char	*path;
	char **entry;
	t_env	*env;

	iter = 0;
	env = env_construct();
	path = NULL;
	shell_level_index = -1;
	while (envp[iter])
	{
		entry = ft_split((char const *)envp[iter], '=');
		env_append(env, entry[KEY_INDEX], entry[VALUE_INDEX]);
		if (ft_strcmp(entry[KEY_INDEX], "PATH") == 0)
			path = entry[VALUE_INDEX];
		if (ft_strcmp(entry[KEY_INDEX], "SHLVL") == 0)
			shell_level_index = iter;
		iter++;
	}
	if (path)
		parse_path(env->path, path);
	env_join(env);
	if (shell_level_index >= 0)
		increment_shell_level(env, shell_level_index);
	return (env);
}

void	env_join(t_env *env)
{
	char	*entry;
	t_cell	cell;
	size_t	iter;

	if (!env || !(env->cells) || !env->size)
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
	env->envp = alloc((env->size + 1 ) * sizeof(*env->envp));
	while (iter < env->size)
	{
		cell = env->cells[iter];
		entry = ft_strjoin(cell.key, "=");
		env->envp[iter++] = ft_strjoin(entry, cell.value);
		ft_free(entry);
	}
	env->envp[iter] = NULL;
}

void	env_append(t_env *env, char *key, char *value)
{
	env_expand(env);
	env->cells[env->size].key = ft_strdup(key);
	env->cells[env->size].value = ft_strdup(value);
	env->size++;
}

void	env_del(t_env *env, char *key)
{
	size_t	index;

	index = env_search(env, key);
	if (index < env->size)
	{
		ft_memmove(env->cells + index, env->cells + index + 1, env->size - index);
		env->size--;
	}
}

void	env_set(t_env *env, char *key, char *new_value)
{
	size_t	index;

	if (ft_strcmp(key, "PATH") == 0)
		parse_path(env->path, new_value);
	if (ft_strcmp(key, "SHLVL") == 0)
		assert(0 && "SHLVL was not implemented to be set yet!");
	index = env_search(env, key);
	if (index < env->size)
	{
		if (env->cells[index].value)
			ft_free(env->cells[index].value);
		env->cells[index].value = ft_strdup(new_value);
	}
	else
		env_append(env, key, new_value);
	env_join(env);
}

char *env_get(t_env *env, char *key)
{
	size_t	index;

	index = env_search(env, key);
	if (index < env->size)
		return (env->cells[index].value);
	return ("");
}

void	env_print(t_env *env)
{
	size_t	iter;
	t_cell	cell;

	if (!env || !env->cells || !env->size)
		return ;
	iter = 0;
	while (iter < env->size)
	{
		cell = env->cells[iter];
		ft_printf("%s=%s\n", cell.key, cell.value);
		iter++;
	}
}
