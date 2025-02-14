/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_xprint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 12:01:39 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/21 16:32:30 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

static char	*yield_hash(t_fstate *state, unsigned int n)
{
	if (n > 0 && (state->flags.detected & HASH_BIT))
	{
		if (state->base.kind == B16L)
			return ("0x");
		else if (state->base.kind == B16U)
			return ("0X");
	}
	return (NULL);
}

static void	__xprintf__internal_nohash(t_fstate *state, unsigned int number,
		int cond)
{
	size_t	length;

	length = digit_lengthu32(number, state->base.length);
	state->flags.perc = clamp(state->flags.perc - length);
	if (state->flags.detected & ZERO_BIT && !(state->flags.detected & DOT_BIT))
		ppad((state->flags.pos_width - length), '0', &state->written);
	else if (cond)
		ppad(state->flags.pos_width, ' ', &state->written);
	else
		ppad(state->flags.pos_width - (length + state->flags.perc), ' ',
			&state->written);
	if (state->flags.detected & DOT_BIT)
		ppad(state->flags.perc, '0', &state->written);
	ft_putu32(number, state, cond);
	if (cond)
		ppad(state->flags.neg_width - state->flags.perc, ' ', &state->written);
	else
		ppad(state->flags.neg_width - length - state->flags.perc, ' ',
			&state->written);
}

static void	__xprintf__internal_hash_finish(t_fstate *state, t_number num,
								char *hash, int cond)
{
	size_t	hslen;

	hslen = ft_strlen(hash);
	if (state->flags.detected & DOT_BIT)
	{
		ft_putstr(hash, &state->written, 2);
		ppad(state->flags.perc, '0', &state->written);
		ft_putu32(num.number, state, cond);
	}
	else
	{
		ft_putstr(hash, &state->written, 2);
		ft_putu32(num.number, state, cond);
	}
	if (cond)
		ppad(state->flags.neg_width - state->flags.perc, ' ', &state->written);
	else
		ppad(state->flags.neg_width - num.length
			- state->flags.perc - hslen, ' ',
			&state->written);
}

static void	__xprintf__internal_hash(t_fstate *state,
							unsigned int number, int cond)
{
	char		*hash;
	size_t		hslen;
	t_number	num;

	num.number = number;
	hash = yield_hash(state, num.number);
	hslen = ft_strlen(hash);
	num.length = digit_lengthu32(num.number, state->base.length);
	state->flags.perc = clamp(state->flags.perc - num.length);
	if (state->flags.detected & ZERO_BIT && !(state->flags.detected & DOT_BIT))
	{
		ft_putstr(hash, &state->written, 2);
		ppad(state->flags.pos_width - num.length - hslen, '0', &state->written);
		hash = NULL;
	}
	else if (cond)
		ppad(state->flags.pos_width, ' ', &state->written);
	else
		ppad(state->flags.pos_width - num.length
			- state->flags.perc - hslen, ' ', &state->written);
	__xprintf__internal_hash_finish(state, num, hash, cond);
}

void	__xprintf(t_fstate *state)
{
	unsigned int	number;
	int				cond;

	number = va_arg(state->args, unsigned int);
	cond = ((number == 0) && (state->flags.detected & DOT_BIT)
			&& (state->flags.perc == 0));
	if (!yield_hash(state, number))
	{
		__xprintf__internal_nohash(state, number, cond);
		return ;
	}
	__xprintf__internal_hash(state, number, cond);
}
