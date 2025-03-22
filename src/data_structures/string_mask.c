/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_mask.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:18:13 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/21 17:46:35 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	mask_expand_anyhow(t_mask *mask)
{
	mask->cap *= VEC_GROWTH_FAC;
	mask->items = ft_realloc(mask->items,
				mask->cap * sizeof(*(mask->items)),
				mask->size * sizeof(*(mask->items)));
}

void	mask_expand(t_mask *mask)
{
	if(mask->size >= mask->cap)
		mask_expand_anyhow(mask);
}

void	mask_push_back(t_mask *mask, u8 item)
{
	mask_expand(mask);
	mask->items[mask->size++] = item;
}

t_mask	*mask_construct(void)
{
	t_mask	*mask;

	mask = alloc(sizeof(*mask));
	mask->items = alloc(VAC_INIT_CAP * sizeof(*mask->items));
	mask->cap = VEC_INIT_CAP;
	mask->size = 0;
	return (mask);
}

void	mask_destroy(t_mask *mask)
{
	if (mask)
	{
		if (mask->items)
			ft_free(mask->items);
		ft_free(mask);
	}
}

void	mask_fill(t_mask *mask, u8 item, size_t size)
{
	mask->size = 0;
	while (mask->size < size)
		mask_push_back(mask, item);
}

void	mask_copy(t_mask *dst, t_mask *src, size_t start)
{
	dst->size = start;
	while (dst->size < src->size)
		mask_push_back(dst, src->items[dst->size]);
}

void	mask_print(t_mask *mask)
{
	ft_printf("Mask size: %u\n", mask->size);
	for (size_t i = 0; i < mask->size; i++)
		ft_printf("%u", mask->items[i]);
}

void	mask_copy_ignore_spaces(t_mask *dst, t_string *src, size_t start)
{
	size_t	i;

	dst->size = start;
	i = start;
	while (i < src->mask->size)
	{
		if (!ft_isspace(src->cstring[i]))
			mask_push_back(dst, src->mask->items[i++]);
		else
			i++;
	}
}
