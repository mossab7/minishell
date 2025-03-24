/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_vconstructors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 16:54:26 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 15:50:54 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	str_join(t_string	*vec, size_t count, ...)
{
	size_t	i;
	va_list	args;

	va_start(args, count);
	i = 0;
	while (i < count)
	{
		str_append(va_arg(args, char *), vec);
		i++;
	}
	va_end(args);
}

t_string	*vstr_construct(size_t count, ...)
{
	t_string	*vec;
	size_t		i;
	va_list		args;

	va_start(args, count);
	vec = str_construct();
	i = 0;
	while (i < count)
	{
		str_append(va_arg(args, char *), vec);
		i++;
	}
	va_end(args);
	return (vec);
}
