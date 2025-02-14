/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 09:00:52 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/24 09:01:23 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static int	fetch_digit(unsigned long *num, char digit, size_t *index)
{
	if (ft_isdigit(digit))
	{
		*num = (*num * 10) + (digit - '0');
		*index = (*index + 1);
		return (1);
	}
	return (0);
}

void	ft_parse_int(t_fstate *state)
{
	unsigned long	num;
	int				sign;

	sign = 1;
	state->flags.detected |= ZERO_BIT * (chop(state->fmt, &state->iter,
				'0') > 0);
	state->flags.detected |= IS_PERC_SET * (ft_isdigit(state->fmt[state->iter])
			&& state->flags.conv == PERC);
	num = 0;
	while (fetch_digit(&num,
			state->fmt[state->iter],
			&state->iter))
		continue ;
	if (state->flags.conv == NEG_WID)
		state->flags.neg_width = (num * sign);
	else if (state->flags.conv == POS_WID)
		state->flags.pos_width = (num * sign);
	else if (state->flags.conv == PERC)
		state->flags.perc = (num * sign);
	else
		state->flags.pos_width = (num * sign);
	if (state->flags.detected & MINUS_BIT)
		state->flags.conv = NEG_WID;
	else
		state->flags.conv = POS_WID;
}
