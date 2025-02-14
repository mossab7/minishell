/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:40:35 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/01 10:16:11 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_printf(const char *fmt, ...)
{
	t_fstate	state;

	if (fmt == NULL || write(1, 0, 0) == -1)
		return (-1);
	init_state(&state, STDOUT_FILENO, fmt);
	va_start(state.args, fmt);
	while (state.fmt[state.iter])
	{
		if (write(1, 0, 0) == -1)
			return (-1);
		ft_memset(&state.flags, 0, sizeof(state.flags));
		if (state.fmt[state.iter] == '%')
		{
			state.iter++;
			parse_flags(&state);
			vprintf__internal(&state);
		}
		else
			ft_putchar(state.fmt[state.iter++], &state.written);
		if (state.written < 0)
			return (state.written);
	}
	va_end(state.args);
	return (state.written);
}

int	ft_fprintf(int fd, const char *fmt, ...)
{
	t_fstate	state;

	if (fmt == NULL || write(fd, 0, 0) == -1)
		return (-1);
	init_state(&state, fd, fmt);
	va_start(state.args, fmt);
	while (state.fmt[state.iter])
	{
		if (write(1, 0, 0) == -1)
			return (-1);
		ft_memset(&state.flags, 0, sizeof(state.flags));
		if (state.fmt[state.iter] == '%')
		{
			state.iter++;
			parse_flags(&state);
			vprintf__internal(&state);
		}
		else
			ft_putchar(state.fmt[state.iter++], &state.written);
		if (state.written < 0)
			return (state.written);
	}
	clean_up_state(&state);
	return (state.written);
}

int	ft_vfprintf(int fd, const char *fmt, va_list args)
{
	t_fstate	state;

	if (fmt == NULL || write(fd, 0, 0) == -1)
		return (-1);
	init_state(&state, fd, fmt);
	va_copy(state.args, args);
	while (state.fmt[state.iter])
	{
		if (write(1, 0, 0) == -1)
			return (-1);
		ft_memset(&state.flags, 0, sizeof(state.flags));
		if (state.fmt[state.iter] == '%')
		{
			state.iter++;
			parse_flags(&state);
			vprintf__internal(&state);
		}
		else
			ft_putchar(state.fmt[state.iter++], &state.written);
		if (state.written < 0)
			return (state.written);
	}
	clean_up_state(&state);
	return (state.written);
}
