#include <zen.h>

/*
lazmoud@e2r7p5:[~/Desktop/programming/projects/Minishell]  export fields="-0 -1 -2                          -111"
lazmoud@e2r7p5:[~/Desktop/programming/projects/Minishell]  ./a.out $fields 
av[0] = ./a.out
av[1] = -0
av[2] = -1
av[3] = -2
av[4] = -111
lazmoud@e2r7p5:[~/Desktop/programming/projects/Minishell]  ./a.out "$fields" 
av[0] = ./a.out
av[1] = -0 -1 -2                          -111
lazmoud@e2r7p5:[~/Desktop/programming/projects/Minishell]  ./a.out '$fields'
av[0] = ./a.out
av[1] = $fields
lazmoud@e2r7p5:[~/Desktop/programming/projects/Minishell]  ./a.out "$fields"
av[0] = ./a.out
av[1] = -0 -1 -2                          -111
lazmoud@e2r7p5:[~/Desktop/programming/projects/Minishell]  ./a.out $fields
av[0] = ./a.out
av[1] = -0
av[2] = -1
av[3] = -2
av[4] = -111
lazmoud@e2r7p5:[~/Desktop/programming/projects/Minishell]  ./a.out "$fields"
*/

void	expand_value(t_token *tok, t_env *env, t_token_array *tokens)
{
	size_t		iter;

	iter = (tok - tokens->items);
	string_expand(env, tok, tokens);
	if (!tok->lexeme->size)
	{
		ft_memmove(tok, (tok + 1), (tokens->size - iter) * sizeof(*tokens->items));
		tokens->size--;
		return ;
	}
}

void	expand(t_env *env, t_token_array *tokens)
{
	t_token			*tok;
	size_t			iter;

	iter = 0;
	while (iter < tokens->size)
	{
		tok = &tokens->items[iter];
		if (tok->type == TOK_WORD)
			expand_value(tok, env, tokens);
		iter++;
	}
}
