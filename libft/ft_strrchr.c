/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:03:20 by lazmoud           #+#    #+#             */
/*   Updated: 2024/10/24 12:29:00 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	ft_strlen_static(unsigned char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	byte;
	unsigned char	*ptr;
	int				end;

	ptr = (unsigned char *)s;
	byte = (unsigned char)c;
	end = ft_strlen_static(ptr);
	while (end >= 0)
	{
		if (ptr[end] == byte)
			return ((char *)ptr + end);
		end--;
	}
	return (NULL);
}
