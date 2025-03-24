/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spadding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:29:46 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/15 15:04:10 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

void	ppad(int len, char byte, int *written)
{
	while (len > 0)
	{
		ft_putchar(byte, written);
		if (written)
		{
			if (*written == -1)
				return ;
		}
		len--;
	}
}
