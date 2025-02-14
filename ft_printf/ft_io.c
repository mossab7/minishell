/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_io.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 09:50:15 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/01 10:22:41 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_io(int fd)
{
	static int	io;

	if (fd == FD_INIT)
		io = STDOUT_FILENO;
	else if (fd == FD_GET)
		return (io);
	else
		io = fd;
	return (io);
}
