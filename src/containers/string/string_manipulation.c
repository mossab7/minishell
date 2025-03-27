/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manipulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:40:18 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/23 17:16:38 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

// ==129791== Invalid read of size 1
// ==129791==    at 0x40DB05: ft_strnstr (in /home/lazmoud/Desktop/programming/projects/Minishell/minishell)
// ==129791==    by 0x403A33: str_search_using_cursor (string_manipulation.c:44)
// ==129791==    by 0x40A35E: find_dollar_sign (string_expand.c:16)
// ==129791==    by 0x40A2E0: extract_key (string_expand.c:62)
// ==129791==    by 0x40993C: tokens_expand (tokens_expand.c:36)
// ==129791==    by 0x408A38: expand_command (expand.c:65)
// ==129791==    by 0x407AD7: parse_command (parse_command.c:60)
// ==129791==    by 0x405D93: parse_primary (parse_and_or_pipe.c:87)
// ==129791==    by 0x405B0C: parse_pipe (parse_and_or_pipe.c:98)
// ==129791==    by 0x405C2C: parse_and_or (parse_and_or_pipe.c:46)
// ==129791==    by 0x404690: build_ast (syntax_tree.c:54)
// ==129791==    by 0x40333C: parser_procced_to_exec (parser.c:16)
// ==129791==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
void	str_append(char *src, t_string *vec)
{
	size_t	index;

	if (!src)
		return ;
	index = 0;
	while (src[index])
	{
		str_push_back(vec, src[index]);
		mask_push_back(vec->mask, 0);
		index++;
	}
}

void	str_overwrite(char *src, t_string *vec)
{
	if (!src)
		return ;
	vec->size = 0;
	vec->cstring[vec->size] = 0;
	if (!*src)
		return ;
	str_append(src, vec);
}

int	str_search_using_cursor(t_string *vec, const char *text)
{
	char	*cursor;

	cursor = ft_strnstr(((const char *) vec->cstring + vec->cursor),
			text, vec->size);
	return ((int)(cursor - vec->cstring));
}

int	str_search(t_string *vec, const char *text)
{
	char	*cursor;

	cursor = ft_strnstr(((const char *) vec->cstring), text, vec->size);
	return ((int)(cursor - vec->cstring));
}

void	str_print(t_string *str)
{
	size_t	i;

	ft_printf("Mask size: %u\n", str->mask->size);
	ft_printf("str  size: %u\n", str->size);
	ft_printf("[%u]# %s\n", str->size, str->cstring);
	ft_printf("     ");
	i = 0;
	while (i < str->mask->size)
		ft_printf("%u", str->mask->items[i++]);
	ft_printf("\n");
}
