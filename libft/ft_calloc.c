/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:53:12 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/30 13:32:32 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static void	ft_bzero_static(void *s, size_t n)
{
	unsigned char	*ptr;

	if (!n)
		return ;
	ptr = s;
	while (1)
	{
		ptr[n - 1] = 0;
		if (n == 1)
			break ;
		n--;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t	storage;
	void	*mem;

	storage = count * size;
	mem = malloc(storage);
	if (mem)
		ft_bzero_static(mem, storage);
	return (mem);
}
