/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:01:39 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 18:03:06 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <libft.h>

bool	is_dir(const char *filename)
{
	struct stat	file_stat;

	if (stat(filename, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
			return (true);
	}
	return (false);
}
