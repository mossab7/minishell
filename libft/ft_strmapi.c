/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:07:01 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/30 13:07:18 by lazmoud          ###   ########.fr       */
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

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*mem;
	unsigned int	idx;

	if (!s)
		return (NULL);
	mem = alloc(len(s) + 1);
	if (!mem)
		return (NULL);
	idx = 0;
	while (s[idx])
	{
		mem[idx] = f(idx, s[idx]);
		idx++;
	}
	mem[idx] = 0;
	return (mem);
}
