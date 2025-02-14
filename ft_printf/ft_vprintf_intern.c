/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vprintf_intern.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:24:23 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/16 18:23:16 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static void	identify_base(t_fstate *state, t_spex spec)
{
	state->base.form = BASE_10;
	state->base.length = 10;
	state->base.kind = B10;
	if (spec == 'x' || spec == 'p')
	{
		state->base.form = BASE_16_L;
		state->base.length = 16;
		state->base.kind = B16L;
	}
	if (spec == 'X')
	{
		state->base.form = BASE_16_U;
		state->base.length = 16;
		state->base.kind = B16U;
	}
}

static void	__unknown_printf(t_spex it, t_fstate *state)
{
	if (!it)
		return ;
	if (state->flags.detected & ZERO_BIT)
		ppad(state->flags.pos_width - 1, '0', &state->written);
	else
		ppad(state->flags.pos_width - 1, ' ', &state->written);
	ft_putchar(it, &state->written);
	ppad(state->flags.neg_width - 1, ' ', &state->written);
}

void	vprintf__internal(t_fstate *state)
{
	t_spex	spec;

	spec = state->fmt[state->iter];
	identify_base(state, spec);
	if (spec == 's')
		__sprintf(state);
	else if (spec == 'i' || spec == 'd')
		__iprintf(state);
	else if (spec == 'x' || spec == 'X' || spec == 'u')
		__xprintf(state);
	else if (spec == 'p')
		__pprintf(state);
	else if (spec == 'c')
		__cprintf(state);
	else
		__unknown_printf(spec, state);
	if (spec)
		state->iter++;
}
