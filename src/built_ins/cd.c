/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:13:36 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/19 11:15:20 by lazmoud          ###   ########.fr       */
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

static int	__cd_success(t_env *env, t_string *resolved_path,
		char *owd)
{
	char		cwd_buff[PATH_MAX];

	if (*owd)
		env_set(env, "OLDPWD", owd);
	if (!getcwd(cwd_buff, PATH_MAX))
	{
		zen_elog("error retrieving current directory: "
			"getcwd: cannot access parent directories: "
			"No such file or directory\n");
		str_destruct(resolved_path);
		return (env->last_command_status = 1);
	}
	env_set(env, "PWD", cwd_buff);
	if (!*owd)
		env_set(env, "OLDPWD", cwd_buff);
	set_pwd(env_get(env, "PWD"));
	str_destruct(resolved_path);
	return (env->last_command_status = 0);
}

int	built_in_cd(t_env *env, int argc, char **dst)
{
	t_string	*resolved_path;
	int			res;
	char		owd[PATH_MAX];
	char		*oldpwd_ptr;

	(void)argc;
	resolved_path = str_construct();
	res = __resolve_path(resolved_path, env, dst[1]);
	if (res != SUCCESS)
		return ((str_destruct(resolved_path))
			, env->last_command_status = 1);
	if (!getcwd(owd, PATH_MAX))
		oldpwd_ptr = env_get(env, "PWD");
	else
		oldpwd_ptr = (char *)owd;
	if (chdir(resolved_path->cstring) == 0)
		return (__cd_success(env, resolved_path, oldpwd_ptr));
	__cd_error(resolved_path, dst[1]);
	return (env->last_command_status = 1);
}
