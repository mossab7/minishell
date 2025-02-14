/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pprint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:28:45 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/21 16:43:21 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static void	zero_pad(t_fstate *state,
					unsigned long number, size_t length)
{
	ft_putstr("0x", &state->written, HEX_PREF_LEN);
	ppad(state->flags.pos_width - length - HEX_PREF_LEN, '0',
		&state->written);
	ft_putu64(number, state);
	ppad(state->flags.neg_width - length - HEX_PREF_LEN, ' ',
		&state->written);
}

static void	__apply_perc(t_fstate *state,
					unsigned long number, size_t length)
{
	ft_putstr("0x", &state->written, HEX_PREF_LEN);
	ppad(state->flags.perc, '0', &state->written);
	if (number == 0 && state->flags.perc == 0)
	{
		ppad(state->flags.neg_width
			- HEX_PREF_LEN - state->flags.perc, ' ',
			&state->written);
		return ;
	}
	ft_putu64(number, state);
	ppad(state->flags.neg_width
		- HEX_PREF_LEN - length - state->flags.perc, ' ',
		&state->written);
}

static void	__pprintf_internal(t_fstate *state,
					unsigned long number, size_t length)
{
	if ((state->flags.detected & ZERO_BIT)
		&& !(state->flags.detected & DOT_BIT))
	{
		zero_pad(state, number, length);
		return ;
	}
	else
	{
		state->flags.pos_width -= (state->flags.perc + HEX_PREF_LEN);
		if (!(number == 0 && !state->flags.perc
				&& (state->flags.detected & DOT_BIT)))
			state->flags.pos_width -= length;
		ppad(state->flags.pos_width, ' ', &state->written);
	}
	if (state->flags.detected & DOT_BIT)
	{
		__apply_perc(state, number, length);
		return ;
	}
	ft_putstr("0x", &state->written, HEX_PREF_LEN);
	ft_putu64(number, state);
	ppad(state->flags.neg_width
		- HEX_PREF_LEN - length - state->flags.perc, ' ',
		&state->written);
}

void	__pprintf(t_fstate *state)
{
	unsigned long	number;
	size_t			length;

	number = va_arg(state->args, unsigned long);
	length = digit_length(number, state->base.length);
	state->flags.perc = clamp(state->flags.perc - length);
	__pprintf_internal(state, number, length);
}
