/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:55:59 by lazmoud           #+#    #+#             */
/*   Updated: 2024/10/25 16:37:16 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	x;
	unsigned char	*ptr;

	x = (unsigned char)c;
	ptr = (unsigned char *)s;
	while (*ptr)
	{
		if (*ptr == x)
			break ;
		ptr++;
	}
	if (*ptr == x)
		return ((char *)ptr);
	return (NULL);
}
