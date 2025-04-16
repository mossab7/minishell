/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:56:31 by lazmoud           #+#    #+#             */
/*   Updated: 2024/10/26 14:13:28 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*s1u;
	unsigned char	*s2u;
	size_t			i;

	i = 0;
	s1u = (unsigned char *)s1;
	s2u = (unsigned char *)s2;
	while (s1u[i] && s2u[i])
	{
		if (s1u[i] != s2u[i])
			return (s1u[i] - s2u[i]);
		i++;
	}
	return (s1u[i] - s2u[i]);
}

int	compare_alpha(const char *s1, const char *s2)
{
	unsigned char	*str1;
	unsigned char	*str2;
	char			c1;
	char			c2;
	size_t			i;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (str1[i] && str2[i])
	{
		c1 = ft_tolower(str1[i]);
		c2 = ft_tolower(str2[i]);
		if (c1 != c2)
			return (c1 - c2);
		if (str1[i] != str2[i])
		{
			if (ft_tolower(str1[i]) == ft_tolower(str2[i]))
				return (str2[i] - str1[i]);
		}
		i++;
	}
	return (str1[i] - str2[i]);
}
