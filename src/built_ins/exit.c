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
