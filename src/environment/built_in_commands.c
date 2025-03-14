#include <zen.h>
#include <libft.h>

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
		return (env->last_command_status = res);
	}
    if (!getcwd(owd, PATH_MAX))
    {
		zen_elog("cd: error getting current directory\n");
		str_destruct(resolved_path);
        return (env->last_command_status = 1);
    }
    if (chdir(resolved_path->cstring) == 0)
    {
        env_set(env, "OLDPWD", owd);
		if (!getcwd(owd, PATH_MAX))
		{
			zen_elog("cd: error getting current directory\n");
			str_destruct(resolved_path);
			return (env->last_command_status = 1);
		}
        env_set(env, "PWD", owd);
		str_destruct(resolved_path);
		return (env->last_command_status = 0);
    }
	if (access(resolved_path->cstring, F_OK) == -1)
		ft_printf("cd: %s: No such file or directory\n", dst[1]);
	else if (access(resolved_path->cstring, X_OK) == -1)
		ft_printf("cd: %s: Permission denied\n", resolved_path->cstring);
	else
		ft_printf("cd: %s: Not a directory\n", resolved_path->cstring);
	str_destruct(resolved_path);
	return (env->last_command_status = 1);
}

int built_in_echo(t_env *env, char **args)
{
	(void)env;
	bool new_line = true;
	size_t i = 0;
	int r;

	args++;
	if(args[i] && !ft_strcmp("-n", args[i]))
	{
		new_line = false;
		i++;
	}
	while (args[i])
	{
		r = ft_printf("%s", args[i]);
		if(r < 0)
			return (env->last_command_status = 1);
		if (args[i + 1])
		{
			r = ft_printf(" ");
			if(r < 0)
				return (env->last_command_status = 1);
		}
		i++;
	}
	if(new_line)
	{
		ft_printf("\n");
		if(r < 0)
			return (env->last_command_status = 1);
	}
	return (env->last_command_status = 0);
}

int built_in_env(t_env *env,char **args)
{
	(void)args;
	env_print(env);
	return (env->last_command_status = 0);
}

int built_in_exit(t_env *env, char **args)
{
	int	code;
	(void)env;
	code = 0;
	if (args && args[1])
		code = ft_atoi(args[1]);
	// BUG: The cleaner does not work properly.
	// ERROR: free() invalid pointer
	cleanup_memory_tracker(get_memory_tracker());
	exit(code);
	return (0);
}

int built_in_set(t_env *env, char **args)
{
	size_t i = 1;

	if (!args[i])
	{
		env_print(env);
		return (env->last_command_status = 0);
	}
	while (args[i])
	{
		char *eq = ft_strchr(args[i], '=');
		if (eq)
		{
			*eq = '\0';
			env_set(env, args[i], eq + 1);
		}
		i++;
	}
	return (env->last_command_status = 0);
}

int built_in_unset(t_env *env, char **args)
{
	size_t i = 1;

	if (!args[i])
		return (env->last_command_status = 0);
	while (args[i])
	{
		env_del(env, args[i]);
		i++;
	}
	return (env->last_command_status = 0);
}
