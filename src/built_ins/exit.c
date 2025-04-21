/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:16:14 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 18:22:21 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static bool	is_numeric(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	built_in_exit(t_env *env, int argc, char **args)
{
	int	code;

	(void)env;
	code = env->last_command_status;
	if (!get_is_insubshell())
		ft_printf("exit\n");
	if (args && args[1])
	{
		if (argc > 2)
		{
			zen_elog("exit: too many arguments \n");
			code = 1;
		}
		else if (!is_numeric(args[1]))
		{
			zen_elog("exit: %s: numeric argument required \n", args[1]);
			code = 2;
		} else
			code = ft_atoi(args[1]);
	}
	cleanup_memory_tracker(get_memory_tracker());
	exit(code);
	return (0);
}
