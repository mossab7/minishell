/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:27:21 by lazmoud           #+#    #+#             */
/*   Updated: 2024/10/22 18:06:57 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	item;
	unsigned char	*items;
	size_t			idx;

	items = (unsigned char *)s;
	item = c;
	idx = 0;
	while (idx < n)
	{
		if (item == items[idx])
			return ((void *)(items + idx));
		idx++;
	}
	return (NULL);
}
