/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:26:02 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/30 17:56:32 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	ft_isdigit_(char c)
{
	return (c >= '0' && c <= '9');
}

static int	ft_derive_sign(char c, int *index)
{
	if (c == '-')
	{
		*index = *index + 1;
		return (-1);
	}
	if (c == '+')
		*index = *index + 1;
	return (1);
}

static int	fetch_digit(unsigned long *num, char digit, int *index, int sign)
{
	if (ft_isdigit_(digit))
	{
		*num = (*num * 10) + (digit - '0');
		if ((*num * 10 + digit - '0') >= 9223372036854775807)
		{
			if (sign == 1)
				*num = -1;
			else
				*num = 0;
			return (0);
		}
		*index = (*index + 1);
		return (1);
	}
	*num = (sign * (*num));
	return (0);
}

int	ft_atoi(const char *str)
{
	unsigned long	num;
	int				index;
	int				sign;

	sign = 1;
	index = 0;
	while (ft_isspace(str[index]))
		index++;
	sign = ft_derive_sign(str[index], &index);
	num = 0;
	while (fetch_digit(&num, str[index], &index, sign))
		continue ;
	return (num);
}
