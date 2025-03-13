#include <zen.h>

static void		print_export(t_env *env)
{
	// TODO: Sort before printing
	cells_sort(env->export_cells);
	printf("Export sorted\n");
	for (size_t i = 0; i < env->export_cells->size; i++)
	{
		printf("declare -x %s", env->export_cells->items[i].key);
		if (env->export_cells->items[i].value)
			printf("=\"%s\"", env->export_cells->items[i].value);
		printf("\n");
	}
}

int built_in_export(t_env *env, char **args)
{
	char **entry;
	size_t i;

	if(!args)
		return (1);
	i = 1;
	if (!args[i])
	{
		print_export(env);
		return (0);
	}
	while (args[i])
	{
		if (*args[i])
		{
			entry = ft_split(args[i], '=');
			// TODO: if an export was called with some like =owko or = only we should throw this error
			// ERROR: bash: export: `=%': not a valid identifier
			// printf("entry[KEY_INDEX]   = %s\n", entry[KEY_INDEX]);
			// printf("entry[VALUE_INDEX] = %s\n", entry[VALUE_INDEX]);
			if (entry[VALUE_INDEX])
			{
				env_set(env, entry[KEY_INDEX], entry[VALUE_INDEX]);
			}
			else
				cells_push_back(env->export_cells, entry[KEY_INDEX], NULL);
		}
		i++;
	}
	return (0);
}
