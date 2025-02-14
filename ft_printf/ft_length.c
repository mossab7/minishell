/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_length.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:28:39 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/16 12:16:19 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

size_t	digit_lengthu32(unsigned int num, int base)
{
	size_t	n;

	n = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num /= base;
		n++;
		if (num == 0)
			break ;
	}
	return (n);
}

size_t	digit_length(unsigned long num, int base)
{
	size_t	n;

	n = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num /= base;
		n++;
		if (num == 0)
			break ;
	}
	return (n);
}

size_t	ft_strlen(char *s)
{
	size_t	len;

	if (!s || !*s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}
