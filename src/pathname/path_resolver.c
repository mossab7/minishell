/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 19:41:19 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 19:51:32 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	walk_back(t_string *path)
{
	char	*loc;

	loc = ft_strrchr(path->cstring, '/');
	if (loc == path->cstring)
	{
		path->cstring[1] = 0;
		path->size = 1;
		return ;
	}
	path->size -= ft_strlen(loc);
	*loc = 0;
}

void	do_nothing(void)
{
}

int	handle_hiphen(t_env *env, char buff[PATH_MAX], char *dst)
{
	char	*value;

	if (ft_strcmp(dst, "-") == 0)
	{
		value = env_get(env, "OLDPWD");
		if (*value)
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
	int		res;
	char	*home;
	char	buffer[PATH_MAX];

	if (!dst || !*dst || ft_strcmp(dst, "~") == 0)
	{
		if (dst == NULL || ft_strcmp(dst, "~") == 0)
		{
			home = env_get(env, "HOME");
			if (!home)
				return ((zen_elog("cd: HOME not set\n")), FAILURE);
			return (__resolve_path(path, env, home));
		}
		return (__resolve_path(path, env, "."));
	}
	res = handle_hiphen(env, buffer, dst);
	if (res == HANDLED)
		return ((str_overwrite(buffer, path)), SUCCESS);
	if (res == FAILURE || res == INVALID_PARAMS_ERROR)
		return (FAILURE);
	str_overwrite(dst, path);
	return (SUCCESS);
}
