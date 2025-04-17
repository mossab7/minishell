/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_insert_defaults.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:03:07 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 21:37:10 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	env_set_defaults(t_env *env)
{
	char	pwd[PATH_MAX];

	ft_memset(pwd, 0, PATH_MAX);
	if (!getcwd(pwd, PATH_MAX))
	{
		zen_elog("error retrieving current directory: "
			"getcwd: cannot access parent directories: "
			"No such file or directory\n");
	}
	insert_if_not_found(env->cells, "PWD", pwd);
	insert_if_not_found(env->export_cells, "PWD", pwd);
	insert_if_not_found(env->cells, "SHLVL", "1");
	insert_if_not_found(env->export_cells, "SHLVL", "1");
	insert_if_not_found(env->cells, "_", (char *)env->program);
	if (!cells_key_exists(env->export_cells, "OLDPWD")
		&& !invalid_oldpwd_detected())
		cells_push_back(env->export_cells,
			"OLDPWD", NULL);
}
