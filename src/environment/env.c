#include <zen.h>

t_env	*env_construct()
{
	t_env	*env;

	env = alloc(sizeof(*env));
	env->cells = alloc(VEC_INIT_CAP * sizeof(*(env->cells)));
	env->cap = VEC_INIT_CAP;
	env->size = 0;
	env->path = strv_construct();
	env->last_command_status = 0;
	return (env);
}

t_env	*env_copy(t_env *env)
{
	t_env	*_env;

	_env = alloc(sizeof(*env));
	_env->size = 0;
	_env->cap = env->cap;
	_env->cells = alloc(_env->cap * sizeof(*(env->cells)));
	_env->path = strv_construct();
	while (_env->size < env->size)
	{
		_env->cells[_env->size].key = ft_strdup(env->cells[_env->size].key);
		_env->cells[_env->size].value = ft_strdup(env->cells[_env->size].value);
		_env->size++;
	}
	return (_env);
}

void	env_expand(t_env *env)
{
	if(env->size >= env->cap)
	{
		env->cap *= VEC_GROWTH_FAC;
		env->cells = ft_realloc(env->cells,
					env->cap * sizeof(*(env->cells)),
					env->size * sizeof(*(env->cells)));
	}
}

void	env_destroy(t_env *env)
{
	size_t	iter;

	if (env)
	{
		iter = 0;
		while (iter < env->size)
		{
			ft_free(env->cells[iter].value);
			ft_free(env->cells[iter].key);
			iter++;
		}
		ft_free(env->cells);
		ft_free(env);
	}
}

size_t	env_search(t_env *env, char *key)
{
	size_t	index;

	index = 0;
	while (index < env->size)
	{
		if (ft_strcmp(env->cells[index].key, key) == 0)
			return (index);
		index++;
	}
	return (index);
}
