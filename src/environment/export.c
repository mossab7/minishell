#include <zen.h>

static	t_token_array *simple_export_tokenizer(char *segment)
{
	// TODO: tokenizer for the input of export
	t_lexer	*lex = lexer_init(segment);
	t_token *tok;
	// lexer_tokenize(lex);
	// lex->cursor: for current character
	ft_free(lex);
	while (lex->source[lex->cursor])
	{
        tok_array_expand(lex->tokens);
        tok = (lex->tokens->items + lex->tokens->size);
		while (ft_isspace(lex->source[lex->cursor]))
			str_push_back(tok->lexeme, lex->source[lex->cursor++]);
		if (ft_isalpha(lex->source[lex->cursor]))
		{
			// TODO: Gather alpha
			str_push_back(tok->lexeme, lex->source[lex->cursor++]);
			while (ft_isalnum(lex->source[lex->cursor]))
				str_push_back(tok->lexeme, lex->source[lex->cursor++]);
			tok->type = TOK_WORD;
		}
		if (ft_isdigit(lex->source[lex->cursor]))
		{
			// TODO: Gather numeric
			str_push_back(tok->lexeme, lex->source[lex->cursor++]);
			while (ft_isalnum(lex->source[lex->cursor]))
				str_push_back(tok->lexeme, lex->source[lex->cursor++]);
			tok->type = TOK_NUM;
		}
		if (ft_ispunct(lex->source[lex->cursor]))
		{
			// TODO: Gather a punct
			tok->type = TOK_SYMBOL;
			str_push_back(tok->lexeme, lex->source[lex->cursor++]);
		}
		lex->tokens->size++;
	}
	return (lex->tokens);
}

static void		print_export(t_env *env)
{
	// TODO: Sort before printing
	cells_sort(env->export_cells);
	for (size_t i = 0; i < env->export_cells->size; i++)
	{
		printf("declare -x %s", env->export_cells->items[i].key);
		if (env->export_cells->items[i].value)
			printf("=\"%s\"", env->export_cells->items[i].value);
		printf("\n");
	}
}

static void set_export(t_env *env, char *new, int *code)
{
	char **entry;
	t_token_array *toks;

	if (new)
	{
		// TODO: Make a simple tokenizer instead of the split
		toks = simple_export_tokenizer(new);
		if (!entry[KEY_INDEX])
		{
			zen_elog("bash: export: `%s': not a valid identifier", new);
			*code = EXIT_SUCCESS;
			return ;
		}
		else if (entry[VALUE_INDEX])
			env_set(env, entry[KEY_INDEX], entry[VALUE_INDEX]);
		else
			cells_push_back(env->export_cells, entry[KEY_INDEX], NULL);
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
