#include <zen.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>


bool _match(char *left,char *right,char *entry)
{
	size_t right_len;
	size_t left_len;
	size_t entry_len = ft_strlen(entry);
	if(right && !left)
	{
		right_len = ft_strlen(right);
		if(strstr(entry + (entry_len - right_len),right))
			return (true);
	}
	else if(!right && left)
	{
		left_len = ft_strlen(left);
		if(ft_strnstr(entry,left,left_len))
			return (true);
	}
	else if(right && left)
	{
		left_len = ft_strlen(left);
		right_len = ft_strlen(right);
		if((strstr(entry + (entry_len - right_len),right)) &&
					(ft_strnstr(entry,left,left_len)))
			return (true);
	}
	return (false);
}


t_string_vector *wildcard_expansion(char *left,char *right)
{
	DIR *dir;
	struct dirent *entry;
	char buffer[PATH_MAX];
	t_string_vector *entries;

	entries = strv_construct();
	getcwd(buffer,PATH_MAX);
	dir = opendir(buffer);
	if(!dir)
		return (NULL);
	while(true)
	{
		entry = readdir(dir);
		if(!entry)
			break;
		//printf("#_>%s\n",entry->d_name);
		if(_match(left,right,entry->d_name))
		{
			strv_push_back(entries,entry->d_name);
		}
	}
	closedir(dir);
	return (entries);
}

