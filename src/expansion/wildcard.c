/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:09:45 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 18:18:33 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static void	handle_dir(char *entry_name, t_string_vector *entries,
		char *pattern)
{
	size_t	pattern_len;

	pattern_len = strlen(pattern);
	if (pattern[pattern_len - 1] == '/')
	{
		pattern[pattern_len - 1] = '\0';
	}
	if (match_pattern(pattern, entry_name) && is_dir(entry_name))
	{
		strv_push_back(entries, entry_name);
	}
	if (pattern[pattern_len - 1] == '\0')
	{
		pattern[pattern_len - 1] = '/';
	}
}

static void	perform_token_expansion(t_token_array *tokens,
		t_token_array *new_tokens_array, t_string_vector *entries,
		size_t cursor)
{
	copy_left_tokens(tokens, new_tokens_array, cursor);
	copy_entries(entries, new_tokens_array, cursor);
	copy_right_tokens(tokens, new_tokens_array, cursor, entries->size);
}

void	wildcard_expand(t_token_array **tokens_array, size_t *cursor)
{
	t_string_vector	*entries;
	t_token_array	*new_tokens_array;
	t_token_array	*tokens;
	size_t			new_size;
	size_t			new_cap;

	tokens = *tokens_array;
	if (should_skip_expansion(tokens, *cursor))
		return ;
	entries = wildcardexpansion(tokens->items[*cursor].lexeme->cstring);
	if (entries->size == 0)
	{
		handle_no_matches(tokens, *cursor, entries);
		return ;
	}
	new_size = tokens->size + entries->size - 1;
	new_cap = tokens->cap + entries->size;
	new_tokens_array = create_new_token_array(new_size, new_cap);
	perform_token_expansion(tokens, new_tokens_array, entries, *cursor);
	*cursor += entries->size - 1;
	strv_destruct(entries);
	*tokens_array = new_tokens_array;
}

static void	process_dir_entries(DIR *dir, t_string_vector *entries,
		char *pattern)
{
	struct dirent	*entry;
	u8				hidden;

	errno = 0;
	while (true)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		hidden = (*(entry->d_name) == '.');
		if ((!hidden || *pattern == '.') && strchr(pattern, '/')
			&& is_dir(entry->d_name))
		{
			handle_dir(entry->d_name, entries, pattern);
			continue ;
		}
		if (match_pattern(pattern, entry->d_name) && (!hidden
				|| *pattern == '.'))
		{
			strv_push_back(entries, entry->d_name);
		}
	}
}

t_string_vector	*wildcardexpansion(char *pattern)
{
	DIR				*dir;
	char			buffer[PATH_MAX];
	t_string_vector	*entries;

	entries = strv_construct();
	dir = open_current_directory(entries, buffer);
	if (!dir)
		return (NULL);
	process_dir_entries(dir, entries, pattern);
	return (check_for_errors(entries, dir));
}
