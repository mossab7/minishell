#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct s_context
{
    bool siginit_received;
    int readline_active;
	t_string	*input;
	t_env		*env;
} t_context;

t_context	**get_context(void);
t_string	*get_context_input(void);
void		init_context(t_string *initial_input, t_env *env);
t_env		*get_context_env(void);
#endif // !CONTEXT_H
