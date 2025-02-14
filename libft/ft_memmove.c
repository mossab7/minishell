/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:08:32 by lazmoud           #+#    #+#             */
/*   Updated: 2024/10/22 21:31:03 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*udst;
	unsigned char	*usrc;
	size_t			idx;

	if (!dst && !src)
		return (NULL);
	udst = (unsigned char *)dst;
	usrc = (unsigned char *)src;
	idx = 0;
	if (udst > usrc)
	{
		while (len-- > 0)
			udst[len] = usrc[len];
		return (dst);
	}
	while (idx < len)
	{
		udst[idx] = usrc[idx];
		idx++;
	}
	return (dst);
}
