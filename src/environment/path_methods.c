/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_methods.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 21:09:13 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/17 17:01:32 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	parse_path(t_string_vector *path, char *src)
{
	char	**paths;

	ft_free(path->cstrings);
	if (!src)
	{
		ft_memset(path, 0, sizeof(*path));
		return ;
	}
	path->cstrings = alloc(VEC_INIT_CAP * sizeof(*path->cstrings));
	path->cap = VEC_INIT_CAP;
	path->size = 0;
	paths = ft_split(src, ':');
	while (paths[path->size])
		strv_push_back(path, paths[path->size]);
	ft_free(paths);
}

t_string	*search_path(t_string_vector *path, char *command)
{
	size_t		iter;
	t_string	*full_path;

	if (ft_strchr(command, '/'))
	{
		if (access(command, F_OK) == 0)
			return (vstr_construct(1, command));
		else
			return (NULL);
	}
	iter = 0;
	if (!path->size)
		return (NULL);
	full_path = str_construct();
	while (iter < path->size)
	{
		str_overwrite(path->cstrings[iter], full_path);
		if (full_path->cstring[full_path->size - 1] != '/')
			str_push_back(full_path, '/');
		str_join(full_path, 1, command);
		if (access(full_path->cstring, F_OK) == 0)
			return (full_path);
		iter++;
	}
	return (str_destruct(full_path), NULL);
}
