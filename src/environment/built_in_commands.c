#include <zen.h>

int built_in_cd(t_env *env, char **dst)
{
	t_string	*resolved_path;
	int			res;
	char		owd[PATH_MAX];

	resolved_path = str_construct();
	res = __resolve_path(resolved_path, env, dst[1]);
	if (res != SUCCESS)
	{
		str_destruct(resolved_path);
		return (1);
	}
    if (!getcwd(owd, PATH_MAX))
    {
		zen_elog("cd: error getting current directory\n");
		str_destruct(resolved_path);
        return (1);
    }
    if (chdir(resolved_path->cstring) == 0)
    {
        env_set(env, "OLDPWD", owd);
		if (!getcwd(owd, PATH_MAX))
		{
			zen_elog("cd: error getting current directory\n");
			str_destruct(resolved_path);
			return (1);
		}
        env_set(env, "PWD", owd);
		str_destruct(resolved_path);
		return (0);
    }
	if (access(resolved_path->cstring, F_OK) == -1)
		ft_printf("cd: %s: No such file or directory\n", dst[1]);
	else if (access(resolved_path->cstring, X_OK) == -1)
		ft_printf("cd: %s: Permission denied\n", resolved_path->cstring);
	else
		ft_printf("cd: %s: Not a directory\n", resolved_path->cstring);
	str_destruct(resolved_path);
	return (1);
}

int built_in_echo(t_env *env, char **args)
{
	(void)env;
	bool new_line = true;
	size_t i = 0;

	args++;
	if(args[i] && !ft_strcmp("-n", args[i]))
	{
		new_line = false;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if(new_line)
		ft_printf("\n");
	return (0);
}

int built_in_env(t_env *env,char **args)
{
	(void)args;
	env_print(env);
	return (0);
}

int built_in_exit(t_env *env, char **args)
{
	int	code;
	(void)env;
	code = 0;
	if (args[1])
		code = ft_atoi(args[1]);
	cleanup_memory_tracker(get_memory_tracker());
	exit(code);
	return (0);
}

int built_in_export(t_env *env, char **args)
{
	char **entry;
	size_t i;

	if(!args)
		return (1);
	i = 1;
	while (args[i])
	{
		if (*args[i])
		{
			entry = ft_split(args[i], '=');
			if (entry[VALUE_INDEX])
				env_set(env, entry[KEY_INDEX], entry[VALUE_INDEX]);
		}
		i++;
	}
	return (0);
}
