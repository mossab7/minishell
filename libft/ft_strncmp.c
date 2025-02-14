/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 11:56:31 by lazmoud           #+#    #+#             */
/*   Updated: 2024/10/26 14:13:28 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <libft.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s1u;
	unsigned char	*s2u;
	size_t			i;

	i = 0;
	s1u = (unsigned char *)s1;
	s2u = (unsigned char *)s2;
	while (i < n && s1u[i] && s2u[i])
	{
		if (s1u[i] != s2u[i])
			return (s1u[i] - s2u[i]);
		i++;
	}
	if (i != n)
		return (s1u[i] - s2u[i]);
	return (0);
}
