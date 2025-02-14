/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:49:03 by lazmoud           #+#    #+#             */
/*   Updated: 2024/11/05 21:24:49 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*mem;

	if (!b)
		return (NULL);
	mem = (unsigned char *)b;
	while (len > 0)
	{
		*(mem++) = (unsigned char)c;
		len--;
	}
	return (b);
}
