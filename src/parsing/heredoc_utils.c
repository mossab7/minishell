/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:45:16 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 19:45:17 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

char	*ft_mkstemp(void)
{
	long	i;
	char	*filename;
	char	*tmp;

	i = 0;
	while (true)
	{
		tmp = ft_itoa(i);
		filename = ft_strjoin("/tmp/heredoc", tmp);
		ft_free(tmp);
		if (access(filename, F_OK) == -1)
			break ;
		ft_free(filename);
		i++;
	}
	return (filename);
}

int	cleanup_on_error(char *filename, int fd)
{
	if (fd >= 0)
		close(fd);
	if (filename)
	{
		unlink(filename);
		ft_free(filename);
	}
	set_context_flag(FLAG_SYNTAX_ERROR);
	return (-1);
}
