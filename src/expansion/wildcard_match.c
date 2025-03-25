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

bool	match_pattern(const char *pattern, const char *str)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern != '*')
	{
		if (*str == '\0')
			return (false);
		if (*pattern != *str)
			return (false);
		return (match_pattern(pattern + 1, str + 1));
	}
	while (*(pattern + 1) == '*')
		pattern++;
	while (*str != '\0')
	{
		if (match_pattern(pattern + 1, str))
			return (true);
		str++;
	}
	return (match_pattern(pattern + 1, str));
}
