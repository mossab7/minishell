#ifndef BUILT_IN_COMMANDS_H
#define BUILT_IN_COMMANDS_H

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif // PATH_MAX

#define BUILT_IN_COMMANDS_COUNT 8
typedef int	(*built_in_command)(t_env *, char **args);

typedef enum e_type
{
    setup,
    teardown
}	t_type;


typedef int	(*built_in_command)(t_env *, char **args);

int built_in_cd(t_env *env, char **args);
int built_in_echo(t_env *env, char **args);
int built_in_env(t_env *env,char **args);
int built_in_exit(t_env *env, char **args);
int built_in_export(t_env *env, char **args);
int	__resolve_path(t_string *path, t_env *env, char *dst);
int built_in_set(t_env *env, char **args);
int built_in_unset(t_env *env, char **args);
#endif // BUILT_IN_COMMANDS_H
