/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_join.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:30:06 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 16:58:46 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static void	free_env_pointers(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		ft_free(envp[i]);
		i++;
	}
	ft_free(envp);
}

static char	**create_env_pointers(t_env *env)
{
	char	**envp;
	char	*entry;
	t_cell	cell;
	size_t	iter;

	envp = alloc((env->cells->size + 1) * sizeof(*envp));
	iter = 0;
	while (iter < env->cells->size)
	{
		cell = env->cells->items[iter];
		entry = ft_strjoin(cell.key, "=");
		envp[iter++] = ft_strjoin(entry, cell.value);
		ft_free(entry);
	}
	envp[iter] = NULL;
	return (envp);
}

void	env_join(t_env *env)
{
	if (!env || !(env->cells) || !env->cells->size)
		return ;
	free_env_pointers(env->envp);
	env->envp = create_env_pointers(env);
}
