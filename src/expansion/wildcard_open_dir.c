/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_open_dir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:16:28 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 18:16:53 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

DIR	*open_current_directory(t_string_vector *entries, char *buffer)
{
	DIR	*dir;

	if (getcwd(buffer, PATH_MAX) == NULL)
	{
		strv_destruct(entries);
		return (NULL);
	}
	dir = opendir(buffer);
	if (!dir)
	{
		strv_destruct(entries);
		return (NULL);
	}
	return (dir);
}
