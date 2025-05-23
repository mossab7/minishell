/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:10:52 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/15 15:26:58 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	built_in_pwd(t_env *env, int argc, char **args)
{
	char		owd[PATH_MAX];
	char		*pwd;
	t_context	*context;

	(void)argc;
	(void)args;
	(void)env;
	context = *get_context();
	pwd = context->pwd;
	if (!*pwd)
	{
		if (!getcwd(owd, PATH_MAX))
		{
			zen_elog("error retrieving current directory: "
				"getcwd: cannot access parent directories: "
				"No such file or directory\n");
			return (1);
		}
		pwd = (char *)owd;
	}
	printf("%s\n", pwd);
	return (0);
}
