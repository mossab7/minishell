/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:40:56 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/08 18:06:53 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <libft.h>

int	in(char *charset, char c)
{
	while (*charset)
	{
		if (*charset == c)
			return (1);
		charset++;
	}
	return (0);
}

static int	count_words(const char *str, char *charset)
{
	int	words_len;
	int	word_len;
	int	i;

	i = 0;
	words_len = 0;
	word_len = 0;
	while (str[i])
	{
		if (in(charset, str[i]))
		{
			if (word_len)
			{
				words_len++;
				word_len = 0;
			}
			i++;
			continue ;
		}
		word_len++;
		i++;
	}
	if (word_len > 0)
		words_len++;
	return (words_len);
}

static int	length_until(char const *s, int idx, char *set)
{
	int	len;

	len = 0;
	while (!in(set, s[idx + len]) && s[idx + len])
		len++;
	return (len);
}

static char	**ft_split_private(char **vec,
	char const *s, char *set, size_t vec_idx)
{
	int		wlength;
	int		i;
	int		k;

	k = 0;
	i = 0;
	while (s[i])
	{
		while (in(set, s[i]) && s[i])
			i++;
		wlength = length_until(s, i, set);
		if (wlength > 0)
		{
			vec[vec_idx] = alloc(wlength + 1);
			if (!vec[vec_idx])
				return (ft_free_2d(vec, vec_idx));
			while (k < wlength && s[i])
				vec[vec_idx][k++] = s[i++];
			vec[vec_idx++][k] = 0;
			k = 0;
		}
	}
	vec[vec_idx] = NULL;
	return (vec);
}

char	**ft_split_set(char const *s, char *set)
{
	char	**array;

	if (!s || !set)
		return (NULL);
	array = alloc(sizeof(char *) * (count_words(s, set) + 2));
	if (!array)
		return (array);
	array = ft_split_private(array, s, set, 0);
	return (array);
}
