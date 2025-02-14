/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:32:35 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/24 09:08:02 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static int	neg_width(t_fstate *state)
{
	state->flags.conv = NEG_WID;
	state->flags.detected |= MINUS_BIT;
	chop(state->fmt, &state->iter, '-');
	if (chop(state->fmt, &state->iter, '+') > 0)
		state->flags.detected |= PLUS_BIT;
	state->flags.neg_width = state->flags.pos_width;
	state->flags.pos_width = 0;
	state->flags.detected &= ~ZERO_BIT;
	return (1);
}

static int	pos_width(t_fstate *state)
{
	state->flags.detected |= PLUS_BIT;
	state->flags.conv = POS_WID;
	chop(state->fmt, &state->iter, '+');
	if (chop(state->fmt, &state->iter, '-') > 0)
	{
		state->flags.conv = NEG_WID;
		state->flags.detected &= ~ZERO_BIT;
		state->flags.detected |= MINUS_BIT;
	}
	ft_parse_int(state);
	return (1);
}

static int	parse_width(t_fstate *state)
{
	if (state->fmt[state->iter] == '-')
		return (neg_width(state));
	else if (state->fmt[state->iter] == '+')
		return (pos_width(state));
	return (0);
}

static int	parse_perc(t_fstate *state)
{
	if (state->fmt[state->iter] == '.')
	{
		state->flags.detected |= DOT_BIT;
		chop(state->fmt, &state->iter, '.');
		state->flags.conv = PERC;
		state->flags.detected |= IS_PERC_SET
			* ft_isdigit(state->fmt[state->iter]);
		ft_parse_int(state);
		return (1);
	}
	return (0);
}

void	parse_flags(t_fstate *state)
{
	while (state->fmt[state->iter])
	{
		if (state->fmt[state->iter] == ' ')
		{
			state->flags.detected |= SPACE_BIT;
			chop(state->fmt, &state->iter, ' ');
		}
		else if (parse_width(state))
			continue ;
		else if (state->fmt[state->iter] == '#')
		{
			state->flags.detected |= HASH_BIT;
			chop(state->fmt, &state->iter, '#');
		}
		else if (parse_perc(state))
			continue ;
		else if (ft_isdigit(state->fmt[state->iter]))
			ft_parse_int(state);
		else
			break ;
	}
}
