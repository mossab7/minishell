/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putu.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:12:55 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/16 18:14:55 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

void	ft_putu64(unsigned long num, t_fstate *state)
{
	if (num > state->base.length - 1)
	{
		ft_putu64(num / (state->base).length, state);
		if (state->written <= 0)
			return ;
	}
	ft_putchar(state->base.form[num % state->base.length], &state->written);
	if (state->written <= 0)
		return ;
}

void	ft_putu32(unsigned int num, t_fstate *state, int cond)
{
	if (cond)
		return ;
	if (num > state->base.length - 1)
	{
		ft_putu32(num / (state->base).length, state, cond);
		if (state->written <= 0)
			return ;
	}
	ft_putchar(state->base.form[num % state->base.length], &state->written);
	if (state->written <= 0)
		return ;
}
