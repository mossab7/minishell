/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:41:00 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/18 17:15:24 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	is_no_nlarg(char *arg)
{
	if (!arg || !*arg || *arg != '-')
		return (0);
	arg++;
	while (*arg == 'n')
		arg++;
	return (*arg == 0);
}

static int	handle_cd_errors(const char *path, char *arg)
{
	if (access(path, F_OK) == -1)
		ft_printf("cd: %s: No such file or directory\n", arg);
	else if (access(path, X_OK) == -1)
		ft_printf("cd: %s: Permission denied\n", path);
	else
		ft_printf("cd: %s: Not a directory\n", path);
	return (1);
}

static int	get_current_directory(char *buffer)
{
	if (!getcwd(buffer, PATH_MAX))
	{
		zen_elog("cd: error getting current directory\n");
		return (1);
	}
	return (0);
}

static int	update_pwd_env(t_env *env, char *old_pwd)
{
	char	current_pwd[PATH_MAX];

	env_set(env, "OLDPWD", old_pwd);
	if (get_current_directory(current_pwd))
		return (1);
	env_set(env, "PWD", current_pwd);
	return (0);
}

int	built_in_cd(t_env *env, char **dst)
{
	t_string	*resolved_path;
	int			res;
	char		old_pwd[PATH_MAX];

	resolved_path = str_construct();
	res = __resolve_path(resolved_path, env, dst[1]);
	if (res != SUCCESS)
	{
		str_destruct(resolved_path);
		return (env->last_command_status = res);
	}
	if (get_current_directory(old_pwd))
	{
		str_destruct(resolved_path);
		return (env->last_command_status = 1);
	}
	if (chdir(resolved_path->cstring) == 0)
	{
		res = update_pwd_env(env, old_pwd);
		str_destruct(resolved_path);
		return (env->last_command_status = res);
	}
	res = handle_cd_errors(resolved_path->cstring, dst[1]);
	str_destruct(resolved_path);
	return (env->last_command_status = res);
}

static size_t	parse_echo_options(char **args, bool *new_line)
{
	size_t	i;

	i = 1;
	*new_line = true;
	if (is_no_nlarg(args[i]))
	{
		*new_line = false;
		i++;
	}
	return (i);
}

static int	print_echo_arguments(char **args, size_t start_index)
{
	int	r;

	r = 0;
	while (args[start_index])
	{
		r = ft_printf("%s", args[start_index]);
		if (r < 0)
			return (r);
		if (args[start_index] && args[start_index + 1])
		{
			r = ft_printf(" ");
			if (r < 0)
				return (r);
		}
		start_index++;
	}
	return (r);
}

static int	print_echo_newline(bool should_print)
{
	int	r;

	r = 0;
	if (should_print)
		r = ft_printf("\n");
	return (r);
}

int	built_in_echo(t_env *env, char **args)
{
	bool	new_line;
	size_t	i;
	int		r;

	(void)env;
	i = parse_echo_options(args, &new_line);
	r = print_echo_arguments(args, i);
	if (r < 0)
		return (env->last_command_status = 1);
	r = print_echo_newline(new_line);
	if (r < 0)
		return (env->last_command_status = 1);
	return (env->last_command_status = 0);
}

int	built_in_env(t_env *env, char **args)
{
	(void)args;
	env_print(env);
	return (env->last_command_status = 0);
}

int	built_in_exit(t_env *env, char **args)
{
	int	code;

	(void)env;
	code = 0;
	if (args && args[1])
		code = ft_atoi(args[1]);
	cleanup_memory_tracker(get_memory_tracker());
	exit(code);
	return (0);
}

int	built_in_set(t_env *env, char **args)
{
	size_t	i;
	char	*eq;

	i = 1;
	if (!args[i])
	{
		env_print(env);
		return (env->last_command_status = 0);
	}
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			*eq = '\0';
			env_set(env, args[i], eq + 1);
		}
		i++;
	}
	return (env->last_command_status = 0);
}

int	built_in_unset(t_env *env, char **args)
{
	size_t	i;

	i = 1;
	if (!args[i])
		return (env->last_command_status = 0);
	while (args[i])
	{
		env_del(env, args[i]);
		i++;
	}
	return (env->last_command_status = 0);
}
