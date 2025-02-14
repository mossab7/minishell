/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:54:36 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/30 13:06:17 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static char	*empty(void)
{
	char	*em;

	em = alloc(1);
	if (!em)
		return (NULL);
	*em = 0;
	return (em);
}

static size_t	len_static(char const *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;

	if (!s)
		return (NULL);
	if (start > len_static(s))
		return (empty());
	if (len > len_static(s + start))
		len = len_static(s + start);
	str = alloc(len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		if (!s[start + i])
			break ;
		i++;
	}
	str[i] = 0;
	return (str);
}
