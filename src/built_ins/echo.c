/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:14:10 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 18:39:46 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static int	__echo_args(char **args, size_t start)
{
	while (args[start])
	{
		if (ft_printf("%s", args[start]) < 0)
			return (FAILURE_);
		if (args[start] && args[start + 1])
		{
			if (ft_printf(" ") < 0)
				return (FAILURE_);
		}
		start++;
	}
	return (OK);
}

int	is_no_nlarg(char *arg)
{
	if (!arg || !*arg || *arg != '-')
		return (0);
	arg++;
	while (*arg == 'n')
		arg++;
	return (*arg == 0);
}

int	built_in_echo(t_env *env, int argc, char **args)
{
	bool	new_line;
	size_t	i;

	(void)env;
	if (argc == 1)
	{
		if (ft_printf("\n") < 0)
			return (env->last_command_status = 1);
		return (env->last_command_status = 0);
	}
	new_line = true;
	i = 1;
	while (is_no_nlarg(args[i]))
	{
		new_line = false;
		i++;
	}
	if (__echo_args(args, i) == FAILURE_)
		return (env->last_command_status = 1);
	if (new_line)
	{
		if (ft_printf("\n") < 0)
			return (env->last_command_status = 1);
	}
	return (env->last_command_status = 0);
}
