/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iprint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:27:42 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/16 17:24:46 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static char	ipsign(t_fstate *state, int *number)
{
	if (*number < 0)
	{
		*number *= -1;
		return ('-');
	}
	if (state->flags.detected & PLUS_BIT)
		return ('+');
	if (state->flags.detected & SPACE_BIT)
		return (' ');
	return (0);
}

static void	__iprintf_internal(t_fstate *state,
				t_number number, int sign, int zero_case)
{
	if (state->flags.detected & ZERO_BIT && !(state->flags.detected & DOT_BIT))
	{
		if (sign)
			ft_putchar(sign, &state->written);
		ppad(state->flags.pos_width - number.length - state->flags.perc, '0',
			&state->written);
	}
	else
	{
		ppad(state->flags.pos_width - number.length
			- state->flags.perc + zero_case,
			' ', &state->written);
		if (sign)
			ft_putchar(sign, &state->written);
	}
	if (state->flags.detected & DOT_BIT)
		ppad(state->flags.perc, '0', &state->written);
	if (zero_case)
	{
		ppad(state->flags.neg_width - (sign != 0), ' ', &state->written);
		return ;
	}
	ft_putu32(number.number, state, 0);
	ppad(state->flags.neg_width - number.length - state->flags.perc, ' ',
		&state->written);
}

void	__iprintf(t_fstate *state)
{
	t_number	number;
	char		sign;
	int			zero_case;

	number.number = va_arg(state->args, int);
	zero_case = ((state->flags.perc == 0) && (state->flags.detected & DOT_BIT)
			&& (number.number == 0));
	sign = ipsign(state, &number.number);
	number.length = digit_lengthu32(number.number,
			state->base.length) + (sign != 0);
	state->flags.perc = clamp(state->flags.perc - number.length + (sign != 0));
	__iprintf_internal(state, number, sign, zero_case);
}
