/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zenlogger.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:43:55 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 06:43:57 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	zen_elog(const char *fmt, ...)
{
	va_list	args;

	va_start(args, fmt);
	ft_fprintf(STDERR_FILENO, "zen: ");
	return (ft_vfprintf(STDERR_FILENO, fmt, args));
}
