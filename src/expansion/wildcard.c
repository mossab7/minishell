#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <zen.h>

static bool	is_dir(const char *filename)
{
	struct stat	file_stat;

	if (stat(filename, &file_stat) == 0)
	{
		if (S_ISDIR(file_stat.st_mode))
		{
			return (true);
		}
	}
	return (false);
}

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

void	copy_left_tokens(t_token_array *tokens, t_token_array *new_tokens_array,
		size_t cursor)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < cursor)
	{
		new_tokens_array->items[i].lexeme = vstr_construct(1,
				tokens->items[i].lexeme->cstring);
		new_tokens_array->items[i].type = tokens->items[i].type;
		if (new_tokens_array->items[i].lexeme->mask == NULL)
			new_tokens_array->items[i].lexeme->mask = mask_construct();
		j = 0;
		while (j < tokens->items[i].lexeme->mask->size)
		{
			mask_push_back(new_tokens_array->items[i].lexeme->mask,
				tokens->items[i].lexeme->mask->items[j]);
			j++;
		}
		i++;
	}
}

void	copy_entries(t_string_vector *entries, t_token_array *new_tokens_array,
		size_t cursor)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < entries->size)
	{
		new_tokens_array->items[cursor + i].lexeme = vstr_construct(1,
				entries->cstrings[i]);
		if (new_tokens_array->items[cursor + i].lexeme->mask == NULL)
			new_tokens_array->items[cursor + i].lexeme->mask = mask_construct();
		new_tokens_array->items[cursor + i].type = TOK_WORD;
		j = 0;
		while (j < ft_strlen(entries->cstrings[i]))
		{
			mask_push_back(new_tokens_array->items[cursor + i].lexeme->mask,
				NOT_QUOTED);
			j++;
		}
		i++;
	}
}

void	copy_right_tokens(t_token_array *tokens,
		t_token_array *new_tokens_array, size_t cursor, size_t entries_size)
{
	size_t	i;
	size_t	j;
	size_t	dest_index;

	i = cursor + 1;
	dest_index = cursor + entries_size;
	while (i < tokens->size && dest_index < new_tokens_array->size)
	{
		new_tokens_array->items[dest_index].lexeme = vstr_construct(1,
				tokens->items[i].lexeme->cstring);
		if (new_tokens_array->items[dest_index].lexeme->mask == NULL)
			new_tokens_array->items[dest_index].lexeme->mask = mask_construct();
		new_tokens_array->items[dest_index].type = tokens->items[i].type;
		j = 0;
		while (j < tokens->items[i].lexeme->mask->size)
		{
			mask_push_back(new_tokens_array->items[dest_index].lexeme->mask,
				tokens->items[i].lexeme->mask->items[j]);
			j++;
		}
		i++;
		dest_index++;
	}
}

static bool	should_skip_expansion(t_token_array *tokens, size_t cursor)
{
	return (cursor >= tokens->size);
}

static void	handle_no_matches(t_token_array *tokens, size_t cursor,
		t_string_vector *entries)
{
	tokens->items[cursor].type = TOK_WORD;
	strv_destruct(entries);
}

static t_token_array	*create_new_token_array(size_t new_size, size_t new_cap)
{
	t_token_array	*new_tokens_array;

	new_tokens_array = alloc(sizeof(t_token_array));
	new_tokens_array->size = new_size;
	new_tokens_array->cap = new_cap;
	new_tokens_array->items = alloc(sizeof(t_token) * new_cap);
	return (new_tokens_array);
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

static t_string_vector	*init_wildcard_expansion(void)
{
	t_string_vector	*entries;

	entries = strv_construct();
	return (entries);
}

static DIR	*open_current_directory(t_string_vector *entries, char *buffer)
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

static t_string_vector	*check_for_errors(t_string_vector *entries, DIR *dir)
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

t_string_vector	*wildcardexpansion(char *pattern)
{
	DIR				*dir;
	char			buffer[PATH_MAX];
	t_string_vector	*entries;

	entries = init_wildcard_expansion();
	if (!entries)
		return (NULL);
	dir = open_current_directory(entries, buffer);
	if (!dir)
		return (NULL);
	process_dir_entries(dir, entries, pattern);
	return (check_for_errors(entries, dir));
}
