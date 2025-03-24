/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_commands.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/24 19:43:41 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_COMMANDS_H
# define BUILT_IN_COMMANDS_H

# include <t_string.h>
# include <env.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif // PATH_MAX

# define BUILT_IN_COMMANDS_COUNT 8

typedef enum e_type
{
	setup,
	teardown
}			t_type;

typedef int	(*t_built_in_command)(t_env *, char **args);

int			built_in_cd(t_env *env, char **args);
int			built_in_echo(t_env *env, char **args);
int			built_in_env(t_env *env, char **args);
int			built_in_exit(t_env *env, char **args);
int			built_in_export(t_env *env, char **args);
int			__resolve_path(t_string *path, t_env *env, char *dst);
int			built_in_set(t_env *env, char **args);
int			built_in_unset(t_env *env, char **args);
#endif // BUILT_IN_COMMANDS_H
