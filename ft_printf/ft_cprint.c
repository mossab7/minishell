/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cprint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 18:19:49 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/16 18:25:32 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

void	__cprintf(t_fstate *state)
{
	char	literal;
	int		length;

	length = 1;
	literal = va_arg(state->args, int);
	if (length > state->flags.perc && state->flags.detected & DOT_BIT)
		length = state->flags.perc;
	ppad(state->flags.pos_width - 1, ' ', &state->written);
	ft_putchar(literal, &state->written);
	ppad(state->flags.neg_width - 1, ' ', &state->written);
}
