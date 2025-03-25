/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:19:16 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 18:19:41 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

t_string_vector	*check_for_errors(t_string_vector *entries, DIR *dir)
{
	if (errno != 0)
	{
		strv_destruct(entries);
		closedir(dir);
		return (NULL);
	}
	closedir(dir);
	return (entries);
}
