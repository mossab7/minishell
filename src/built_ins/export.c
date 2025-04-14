/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:41:30 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 20:06:37 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

static	void	export_find_join(t_env *env, char *key, char *value)
{
	t_string	*new_value;
	size_t		index;

	index = cells_search(env->export_cells, key);
	if (index < env->cells->size)
	{
		new_value = vstr_construct(2, env->export_cells->items[index].value,
				value);
		env_set(env, key, new_value->cstring);
		str_destruct(new_value);
	}
	else
		env_set(env, key, value);
}

static void	process_export_args(t_env *env, t_token_array *toks)
{
	t_token	*key;
	t_token	*assign_op;
	t_token	*value;

	key = (toks->items + 0);
	if (toks->size == 1)
	{
		if (!cells_key_exists(env->export_cells, key->lexeme->cstring))
			cells_push_back(env->export_cells,
				key->lexeme->cstring, NULL);
		return ;
	}
	assign_op = (toks->items + 1);
	value = (toks->items + 2);
	if (assign_op->type == TOK_EQ)
		env_set(env, key->lexeme->cstring, value->lexeme->cstring);
	else if (assign_op->type == TOK_PEQ)
		export_find_join(env, key->lexeme->cstring, value->lexeme->cstring);
}

static void	print_export(t_env *env)
{
	size_t	i;

	i = 0;
	if (!env->export_cells->size)
		return ;
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

static void	set_export(t_env *env, char *new, int *code)
{
	t_token_array	*toks;

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
		process_export_args(env, toks);
		toks_destroy(toks);
	}
}

int	built_in_export(t_env *env, int argc, char **args)
{
	int		code;
	size_t	i;

	(void)argc;
	if (!args)
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
