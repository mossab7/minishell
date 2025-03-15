#include <zen.h>

static void		print_export(t_env *env)
{
	size_t	i;

	i = 0;
	cells_sort(env->export_cells);
	while (i < env->export_cells->size)
	{
		printf("declare -x %s", env->export_cells->items[i].key);
		if (env->export_cells->items[i].value)
			printf("=\"%s\"", env->export_cells->items[i].value);
		printf("\n");
		i++;
	}
}

static void set_export(t_env *env, char *new, int *code)
{
	t_token_array *toks;

	if (new)
	{
		toks = simple_export_tokenizer(new);
		if (!toks)
		{
			zen_elog("export: `%s': not a valid identifier\n", new);
			*code = EXIT_FAILURE;
			toks_destroy(toks);
			return ;
		}
		if (toks->size == 1)
			cells_push_back(env->export_cells, ((*toks->items).lexeme->cstring), NULL);
		// TODO: Check for += and join. 
		tok_array_print(toks);
		toks_destroy(toks);
	}
}

int built_in_export(t_env *env, char **args)
{
	int	code;
	size_t i;

	if(!args)
		return (1);
	i = 1;
	code = EXIT_SUCCESS;
	if (!args[i])
	{
		print_export(env);
		return (0);
	}
	while (args[i])
	{
		set_export(env, args[i], &code);
		i++;
	}
	return (code);
}
