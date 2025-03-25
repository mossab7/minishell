/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:13:36 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 18:31:56 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static void	__cd_error(t_string *resolved_path, char *dst)
{
	if (access(resolved_path->cstring, F_OK) == -1)
		zen_elog("cd: %s: No such file or directory\n", dst);
	else if (access(resolved_path->cstring, X_OK) == -1)
		zen_elog("cd: %s: Permission denied\n", resolved_path->cstring);
	else
		zen_elog("cd: %s: Not a directory\n", resolved_path->cstring);
	str_destruct(resolved_path);
}

static int	__cd_success(t_env *env,
				t_string *resolved_path, char owd[PATH_MAX])
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

int	built_in_cd(t_env *env, char **dst)
{
	t_string	*resolved_path;
	int			res;
	char		owd[PATH_MAX];

	resolved_path = str_construct();
	res = __resolve_path(resolved_path, env, dst[1]);
	if (res != SUCCESS)
	{
		str_destruct(resolved_path);
		return (env->last_command_status = 1);
	}
	if (!getcwd(owd, PATH_MAX))
	{
		zen_elog("cd: error getting current directory\n");
		str_destruct(resolved_path);
		return (env->last_command_status = 1);
	}
	if (chdir(resolved_path->cstring) == 0)
		return (__cd_success(env, resolved_path, owd));
	__cd_error(resolved_path, dst[1]);
	return (env->last_command_status = 1);
}
