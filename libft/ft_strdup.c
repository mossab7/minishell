/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:07:04 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/30 13:43:39 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	ft_strlen_static(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

static char	*ft_strcpy_static(char *buff, const char *other)
{
	int	index;

	index = 0;
	while (other[index])
	{
		buff[index] = other[index];
		index++;
	}
	buff[index] = 0;
	return (buff);
}

char	*ft_strdup(const char *s1)
{
	char	*dup;
	size_t	size;

	size = ft_strlen_static(s1);
	dup = alloc(size + 1);
	if (!dup)
		return (NULL);
	dup = ft_strcpy_static(dup, s1);
	return (dup);
}
