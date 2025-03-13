#include <libft.h>

t_string *ft_readline(const char *prompt)
{
	t_string	*line;
	char 		*tmp;

	tmp = readline(prompt);
	if (!tmp)
		return (NULL);
	line = str_construct();
	str_append(tmp, line);
	free(tmp);
	return (line);
}
