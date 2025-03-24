/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zen.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:23:54 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/22 20:23:58 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
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
#include <assert.h>
#include <context.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <termios.h>

#define VEC_INIT_CAP 16
#define EMPTY_VALUE ""
#define VEC_GROWTH_FAC 2

typedef enum e_error_res
{
	FAILURE,
	HANDLED,
	INVALID_PARAMS_ERROR,
	NOT_HANDLED,
} t_error_res;

void		lex_quote(t_token *tok, u8 *state, char quote, t_stack *stacks[QUOTING_STACKS_SIZE]);
void		string_expand(t_env *env, t_token_array *tokens, size_t *cursor);
void		_string_expand(t_env *env, t_string *string);
void		expand(t_env *env, t_token_array **tokens);
int			zen_elog(const char *fmt, ...);
t_string	*zen_prompt(t_env *env);
void		tokens_field_split(t_token_array *tokens, size_t index);
void		wildcard_expand(t_token_array **tokens, size_t *cursor);
void		ft_quote_error(t_parser *parser);
#endif // ZEN_H
