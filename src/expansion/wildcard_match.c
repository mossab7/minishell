/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:20:51 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 17:21:05 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

bool	match_pattern(t_string *pattern, const char *str, int index)
{
	if (pattern->cstring[index] == '\0')
		return (*str == '\0');
	str_print(pattern);
	if (pattern->cstring[index] != '*')
	{
		if (*str == '\0')
			return (false);
		if (pattern->cstring[index] != *str)
			return (false);
		return (match_pattern(pattern, str + 1, ++index));
	}
	while (pattern->cstring[index + 1] == '*' && (pattern->mask->items[index + 1] & NOT_QUOTED))
		index++;
	while (*str != '\0')
	{
		if (match_pattern(pattern, str, ++index))
			return (true);
		str++;
	}
	return (match_pattern(pattern, str, ++index));
}
