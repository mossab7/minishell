/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eshould_field_split.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:01:11 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/28 14:01:22 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	export_must_field_split(t_string *entry)
{
	size_t	i;

	i = 0;
	while (i < entry->size && entry->cstring[i] != '=')
	{
		if (entry->mask->items[i] & EXPANDED)
			return (1);
		i++;
	}
	return (0);
}
