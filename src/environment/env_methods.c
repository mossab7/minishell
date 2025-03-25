/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_methods.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:41:14 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 21:29:27 by lazmoud          ###   ########.fr       */
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
	ft_free(env->export_cells->items[index].value);
	env->export_cells->items[index].value = ft_itoa(lvl);
}

static void	process_env_entries(t_env *env, const char *envp[])
{
	size_t	iter;
	char	**entry;

	iter = 0;
	while (envp[iter])
	{
		entry = ft_split((char const *)envp[iter], '=');
		if (ft_strcmp(entry[KEY_INDEX], "_") != 0)
			env_append_both(env, entry[KEY_INDEX], entry[VALUE_INDEX]);
		if (ft_strcmp(entry[KEY_INDEX], "PATH") == 0)
			parse_path(env->path, entry[VALUE_INDEX]);
		if (ft_strcmp(entry[KEY_INDEX], "SHLVL") == 0)
			increment_shell_level(env, iter);
		ft_free(entry[KEY_INDEX]);
		ft_free(entry[VALUE_INDEX]);
		ft_free(entry);
		iter++;
	}
}

t_env	*env_parse(const char *envp[], const char *program)
{
	t_env	*env;

	env = env_construct();
	env->program = program;
	process_env_entries(env, envp);
	env_set_defaults(env);
	env_join(env);
	return (env);
}
