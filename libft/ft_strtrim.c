/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 20:30:43 by lazmoud           #+#    #+#             */
/*   Updated: 2025/01/30 13:06:00 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	len(char const *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static int	exists(char c, char const *set)
{
	int	i;

	if (!set)
		return (0);
	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			break ;
		i++;
	}
	return ((set[i] == c) && c);
}

static void	trim_(char **start, char **end, char const *set)
{
	while (*end > *start)
	{
		if (exists(**end, set))
			*end -= 1;
		else
			break ;
	}
	while (*start < *end)
	{
		if (exists(**start, set))
			*start += 1;
		else
			break ;
	}
}

static char	*trim_core_private(size_t storage, char *start, char *end)
{
	char	*new;
	size_t	idx;

	idx = 0;
	new = alloc(storage + 1);
	if (!new)
		return (NULL);
	if (start == end)
	{
		new[idx] = 0;
		return (new);
	}
	while (start <= end)
	{
		new[idx] = *start;
		idx++;
		start++;
	}
	new[idx] = 0;
	return (new);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*start;
	char	*end;
	size_t	storage;

	if (!s1)
		return (NULL);
	start = (char *)s1;
	end = (char *)(&s1[len(s1) - 1]);
	trim_(&start, &end, set);
	storage = (int)(end - start + 1);
	return (trim_core_private(storage, start, end));
}
