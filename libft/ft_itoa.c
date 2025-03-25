/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:09:18 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/30 13:03:55 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	digit_length_base(unsigned long num)
{
	int	n;

	n = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num /= 10;
		n++;
		if (num == 0)
			return (n);
	}
	return (n);
}

static char	*fill_number(unsigned long num, int n, int digcount)
{
	char	*ascii;

	ascii = alloc(digcount + 1);
	if (!ascii)
		return (NULL);
	ascii[digcount--] = 0;
	if (num == 0)
	{
		ascii[digcount--] = '0';
		return (ascii);
	}
	while (num > 0)
	{
		if (num == 0)
			break ;
		ascii[digcount--] = '0' + (num % 10);
		num /= 10;
	}
	if (n < 0)
		ascii[digcount--] = '-';
	return (ascii);
}

char	*ft_itoa(int n)
{
	unsigned long	num;
	int				nsz;

	nsz = 0;
	num = n;
	if (n < 0)
		nsz++;
	if (nsz)
		num = -num;
	nsz += digit_length_base(num);
	return (fill_number(num, n, nsz));
}
