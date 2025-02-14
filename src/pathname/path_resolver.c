/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:41:19 by lazmoud           #+#    #+#             */
/*   Updated: 2025/02/07 22:01:37 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	walk_back(t_string *path)
{
	char *loc = ft_strrchr(path->cstring, '/');
	if (loc == path->cstring)
	{
		// root a3mi
		path->cstring[1] = 0;
		path->size = 1;
		return ;
	}
	path->size -= ft_strlen(loc);
	*loc = 0;
}

void	do_nothing()
{
}

int	handle_hiphen(t_env *env, char buff[PATH_MAX], char *dst)
{
	char		*value;

	if (ft_strcmp(dst, "-") == 0)
	{
		value = env_get(env, "OLDPWD");
		if (value)
		{
			strcpy(buff, value);
			return (HANDLED);
		}
		else
		{
			zen_elog("cd: OLDPWD not set\n");
			return (FAILURE);
		}
	}
	if (*dst == '-')
	{
		zen_elog("cd: %s: invalid option\n", dst);
		return (INVALID_PARAMS_ERROR);
	}
	return (NOT_HANDLED);
}

int	__resolve_path(t_string *path, t_env *env, char *dst)
{
	int			res;
	char		*home;
	char		buffer[PATH_MAX];

	if (!dst || !*dst)
	{
		if (dst == NULL)
		{
			home = env_get(env, "HOME");
			if (!home)
			{
				zen_elog("cd: HOME not set\n");
				return (FAILURE);
			}
			return (__resolve_path(path, env, home));
		}
		return (__resolve_path(path, env, "."));
	}
	res = handle_hiphen(env, buffer, dst);
	if (res == HANDLED)
	{
		str_overwrite(buffer, path);
		return (SUCCESS);
	}
	if (res == FAILURE || res == INVALID_PARAMS_ERROR)
		return (FAILURE);
	str_overwrite(dst, path);
	return (SUCCESS);
}

int	__resolve_path__(t_string *path, t_env *env, char *dst)
{
	char		**args;
	int			res;
	char		*home;
	char		buffer[PATH_MAX];
	size_t		i;

	if (!dst || !*dst)
	{
		if (dst == NULL)
		{
			home = env_get(env, "HOME");
			if (!home)
			{
				zen_elog("cd: HOME not set\n");
				return (FAILURE);
			}
			return (__resolve_path(path, env, home));
		}
		return (__resolve_path(path, env, "."));
	}
	res = handle_hiphen(env, buffer, dst);
	if (res == HANDLED)
	{
		str_overwrite(buffer, path);
		return (SUCCESS);
	}
	if (res == FAILURE || res == INVALID_PARAMS_ERROR)
		return (FAILURE);
	if(!getcwd(buffer, PATH_MAX))
	{
		zen_elog("cd: could not get current working dir\n");
		return (FAILURE);
	}
	args = ft_split(dst,'/');
	if (!*args)
	{
		str_overwrite("/", path);
		return (SUCCESS);
	}
	i = 0;

	if (!ft_strcmp(args[i], "..") && *dst != '/')
	{
		str_join(path, 1, buffer);
		walk_back(path);
		i++;
	}
	else if (!ft_strcmp(args[i], "."))
	{
		str_join(path, 1, buffer);
		i++;
	}
	while (args[i])
	{
		if (!ft_strcmp(args[i], "."))
			do_nothing();
		else if (!ft_strcmp(args[i], ".."))
			walk_back(path);
		else
		{
			if (path->cstring[path->size - 1] != '/' && i)
				str_join(path, 2, "/", args[i]);
			else if (*dst == '/' && !i)
				str_join(path, 2, "/", args[i]);
			else
				str_join(path, 1, args[i]);
		}
		i++;
	}
	return (SUCCESS);
}
