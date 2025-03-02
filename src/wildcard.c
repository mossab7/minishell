#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
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

t_string_vector	*wildcardexpansion(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	u8				hidden;
	char			buffer[PATH_MAX];
	t_string_vector	*entries;

	printf("--->%s\n",pattern);
	entries = strv_construct();
	if (!entries)
		return (NULL);
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
	errno = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		hidden = (*(entry->d_name) == '.');
		if (match_pattern(pattern, entry->d_name) && (!hidden || *pattern == '.'))
			strv_push_back(entries, entry->d_name);
	}
	if (errno != 0)
	{
		strv_destruct(entries);
		closedir(dir);
		return (NULL);
	}
	closedir(dir);
	return (entries);
}
