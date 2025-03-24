/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:27:12 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/01 10:20:19 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

void	__sprintf(t_fstate *state)
{
	char	*literal;
	int		length;

	literal = va_arg(state->args, char *);
	if (!literal)
		literal = NULL_LIT;
	length = ft_strlen(literal);
	if (length > state->flags.perc && state->flags.detected & DOT_BIT)
		length = state->flags.perc;
	ppad(state->flags.pos_width - length, ' ', &state->written);
	ft_putstr(literal, &state->written, length);
	ppad(state->flags.neg_width - length, ' ', &state->written);
}

void	ft_putstr(char *s, int *count, int length)
{
	ssize_t	ret;

	if (!s || !*s || !length)
		return ;
	ret = write(ft_io(FD_GET), s, length);
	if (ret > 0)
	{
		*count += ret;
		return ;
	}
	*count = ret;
}

void	ft_putchar(char c, int *count)
{
	if ((write(ft_io(FD_GET), &c, 1)) <= 0)
	{
		if (count)
			*count = -1;
		return ;
	}
	if (count)
		*count += 1;
}
