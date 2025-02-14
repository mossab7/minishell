/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:34:14 by lazmoud           #+#    #+#             */
/*   Updated: 2024/10/22 18:48:01 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*s1u;
	unsigned char	*s2u;
	size_t			idx;

	if (n == 0)
		return (0);
	idx = 0;
	s1u = (unsigned char *)s1;
	s2u = (unsigned char *)s2;
	while (idx < n)
	{
		if (s1u[idx] != s2u[idx])
			break ;
		idx++;
	}
	if (idx == n)
		return (0);
	return (s1u[idx] - s2u[idx]);
}
