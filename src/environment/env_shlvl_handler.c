/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_shlvl_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 04:13:13 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/17 04:13:22 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	increment_shell_level(t_env *env)
{
	long	lvl;
	int		res;
	int		index;

	index = env->cells->size - 1;
	res = ft_atol_base(env->cells->items[index].value, BASE_10, &lvl);
	lvl++;
	if (lvl > SHLVL_MAX && (res != NON && res != OVER_FLOW_DETECTED))
		zen_elog("warning: shell level (%d) too high, resetting to 1\n", lvl);
	if (res == NON || res == OVER_FLOW_DETECTED || lvl > SHLVL_MAX)
		lvl = 1;
	if (lvl < 0)
		lvl = 0;
	ft_free(env->cells->items[index].value);
	env->cells->items[index].value = ft_itoa(lvl);
	ft_free(env->export_cells->items[index].value);
	env->export_cells->items[index].value = ft_itoa(lvl);
}
