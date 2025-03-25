/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 20:06:09 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/30 13:05:24 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	len(char const *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1l;
	size_t	s2l;
	char	*new;
	int		iter;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	s1l = len(s1);
	s2l = len(s2);
	iter = 0;
	new = alloc(s1l + s2l + 1);
	while (s1[iter])
	{
		new[iter] = s1[iter];
		iter++;
	}
	while (s2[iter - s1l])
	{
		new[iter] = s2[iter - s1l];
		iter++;
	}
	new[iter] = 0;
	return (new);
}
