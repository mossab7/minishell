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

t_string	*ft_readline(const char *prompt)
{
	t_string	*line;
	char		*tmp;

	tmp = readline(prompt);
	if (!tmp)
		return (NULL);
	line = str_construct();
	str_append(tmp, line);
	free(tmp);
	return (line);
}
