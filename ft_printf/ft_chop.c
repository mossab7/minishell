/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:31:23 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/15 14:41:08 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	chop(const char *fmt, size_t *index, char c)
{
	int	chopped;

	chopped = 0;
	while (fmt[*index + chopped] == c && fmt[*index + chopped])
		chopped++;
	*index += chopped;
	return (chopped);
}
