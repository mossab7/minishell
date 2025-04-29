/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:20:51 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/16 22:21:57 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

bool	match_pattern(t_string *pattern, const char *str, int index)
{
	if (pattern->cstring[index] == '\0')
		return (*str == '\0');
	if (pattern->cstring[index] == '*'
		&& (pattern->mask->items[index] & NOT_QUOTED))
	{
		while (pattern->cstring[index + 1] == '*'
			&& (pattern->mask->items[index + 1] & NOT_QUOTED))
			index++;
		while (true)
		{
			if (match_pattern(pattern, str, index + 1))
				return (true);
			if (*str == '\0')
				break ;
			str++;
		}
		return (false);
	}
	else
	{
		if (*str == '\0' || pattern->cstring[index] != *str)
			return (false);
		return (match_pattern(pattern, str + 1, index + 1));
	}
}
