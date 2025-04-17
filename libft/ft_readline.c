/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:15:58 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 20:15:59 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <libft.h>

static void	switch_echo_state(int a)
{
	static int	stdout_backup;
	static int	dev_null;

	if (isatty(STDIN_FILENO))
		return ;
	if (a == SET_NO_ECHO)
	{
		stdout_backup = dup(STDOUT_FILENO);
		dev_null = open(DEV_NULL, O_WRONLY);
		if (dev_null != -1)
		{
			dup2(dev_null, STDOUT_FILENO);
			close(dev_null);
		}
	}
	else if (a == RESTORE)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}

t_string	*ft_readline(const char *prompt)
{
	t_string	*line;
	char		*tmp;

	switch_echo_state(SET_NO_ECHO);
	tmp = readline(prompt);
	switch_echo_state(RESTORE);
	line = str_construct();
	str_append(tmp, line);
	free(tmp);
	return (line);
}
