/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:16:27 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/30 13:07:07 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <libft.h>

void	*ft_realloc(void *ptr, size_t new_sz, size_t old_sz)
{
	char	*new;
	size_t	index;

	index = 0;
	new = alloc(new_sz);
	if (new == NULL)
	{
		ft_free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (new);
	while (index < old_sz)
	{
		new[index] = ((char *)ptr)[index];
		index++;
	}
	ft_free(ptr);
	return (new);
}
