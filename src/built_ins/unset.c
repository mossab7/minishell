/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 18:15:44 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 18:22:21 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	built_in_unset(t_env *env, int argc, char **args)
{
	size_t	iter;

	(void)argc;
	iter = 1;
	while (args[iter])
	{
		cells_remove(env->cells, args[iter]);
		cells_remove(env->export_cells, args[iter]);
		iter++;
	}
	return (0);
}
