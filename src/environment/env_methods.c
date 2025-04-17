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

static void	set_pwd(t_env *env)
{
	char		pwd[PATH_MAX];

	if (!getcwd(pwd, PATH_MAX))
	{
		zen_elog("error retrieving current directory: "
			"getcwd: cannot access parent directories: "
			"No such file or directory\n");
	}
	env_append_both(env, "PWD", pwd);
}

static int	set_oldpwd(t_env *env, char *oldpwd)
{
	static int	is_invalid_found;

	if (!env && !oldpwd)
		return (is_invalid_found);
	if (is_dir(oldpwd))
	{
		printf("SET: oldpwd = %s\n", oldpwd);
		env_append_both(env, "OLDPWD", oldpwd);
		return (is_invalid_found);
	}
	is_invalid_found = 1;
	return (is_invalid_found);
}

int	invalid_oldpwd_detected(void)
{
	return (set_oldpwd(NULL, NULL));
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
		{
			if (ft_strcmp(entry[KEY_INDEX], "OLDPWD") == 0)
				set_oldpwd(env, entry[VALUE_INDEX]);
			else if (ft_strcmp(entry[KEY_INDEX], "PWD") == 0)
				set_pwd(env);
			else
				env_append_both(env, entry[KEY_INDEX], entry[VALUE_INDEX]);
		}
		if (ft_strcmp(entry[KEY_INDEX], "PATH") == 0)
			parse_path(env->path, entry[VALUE_INDEX]);
		if (ft_strcmp(entry[KEY_INDEX], "SHLVL") == 0)
			increment_shell_level(env);
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
