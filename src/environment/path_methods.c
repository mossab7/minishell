/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_methods.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 21:09:13 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/22 11:05:46 by lazmoud          ###   ########.fr       */
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

static t_string	*resolve_abs_path(char *cmd, int *code)
{
	if (is_dir(cmd))
		zen_elog("%s: Is a directory\n", cmd);
	else if (access(cmd, F_OK | X_OK))
		zen_elog("%s: Permission denied\n", cmd);
	else
		return (vstr_construct(1, cmd));
	*code = PERM_DENIED;
	return (NULL);
}

t_string	*search_path(t_string_vector *path, char *cmd, int *code)
{
	size_t		iter;
	t_string	*full_path;

	if (ft_strchr(cmd, '/'))
		return (resolve_abs_path(cmd, code));
	iter = 0;
	full_path = str_construct();
	while (iter < path->size)
	{
		str_overwrite(path->cstrings[iter], full_path);
		if (full_path->cstring[full_path->size - 1] != '/')
			str_push_back(full_path, '/');
		str_join(full_path, 1, cmd);
		if (access(full_path->cstring, F_OK) == 0)
			return (full_path);
		iter++;
	}
	return (str_destruct(full_path),
		zen_elog("%s: command not found\n", cmd), NULL);
}
