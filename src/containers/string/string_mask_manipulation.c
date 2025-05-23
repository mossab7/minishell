/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_mask_manipulation.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:42:29 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 15:42:55 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	mask_fill(t_mask *mask, t_u8 item, size_t size)
{
	mask->size = 0;
	while (mask->size < size)
		mask_push_back(mask, item, 0);
}

void	mask_copy(t_mask *dst, t_mask *src, size_t start)
{
	dst->size = start;
	while (dst->size < src->size)
		mask_push_back(dst, src->items[dst->size], src->ids[dst->size]);
}

void	mask_print(t_mask *mask)
{
	size_t	i;

	ft_printf("Mask size: %u\n", mask->size);
	i = 0;
	while (i < mask->size)
		ft_printf("%u", mask->items[i++]);
}
