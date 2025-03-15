#ifndef ZEN_H
#define ZEN_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <signals.h>
#include <syntax_tree.h>
#include <limits.h>
#include <env.h>
#include <ft_printf.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <t_string.h>
#include <stack.h>
#include <zenint.h>
#include <built_in_commands.h>
#include <execute.h>
#include <t_string_vector.h>
#include <context.h>
#include <assert.h>
#include <sys/stat.h> 
#define VEC_INIT_CAP 16
#define VEC_GROWTH_FAC 2
typedef enum e_error_res
{
	FAILURE,
	HANDLED,
	INVALID_PARAMS_ERROR,
	NOT_HANDLED,
} t_error_res;

void	string_expand(t_env *env, t_token *tok, t_token_array *tokens);
void	expand(t_env *env, t_token_array *tokens);
int		zen_elog(const char *fmt, ...);
#endif // ZEN_H
