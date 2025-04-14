/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zen.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:23:54 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/25 18:38:00 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ZEN_H
# define ZEN_H

# include <assert.h>
# include <built_in_commands.h>
# include <context.h>
# include <dirent.h>
# include <env.h>
# include <errno.h>
# include <execute.h>
# include <fcntl.h>
# include <ft_printf.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <signals.h>
# include <stack.h>
# include <stdio.h>
# include <syntax_tree.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <t_string.h>
# include <t_string_vector.h>
# include <termios.h>
# include <unistd.h>
# include <zenint.h>

# define VEC_INIT_CAP 16
# define EMPTY_VALUE ""
# define VEC_GROWTH_FAC 2

typedef enum e_error_res
{
	FAILURE,
	HANDLED,
	INVALID_PARAMS_ERROR,
	NOT_HANDLED,
}				t_error_res;

void			lex_quote(t_token *tok, t_u8 *state, char quote,
					t_stack *stacks[QUOTING_STACKS_SIZE]);
void			tokens_expand(t_env *env,
					t_token_array *tokens, size_t *cursor);
void			string_expand(t_env *env, t_string *string);
void			expand(t_env *env, t_token_array **tokens);
int				zen_elog(const char *fmt, ...);
t_string		*zen_prompt(t_env *env);
void			field_split(t_token_array **tokens_array, int is_export);
void			wildcard_expand(t_token_array **tokens, size_t *cursor);
void			ft_quote_error(t_parser *parser);
t_string		*extract_key(t_string *string);
bool			match_pattern(const char *pattern, const char *str);
DIR				*open_current_directory(t_string_vector *entries, char *buffer);
t_string_vector	*check_for_errors(t_string_vector *entries, DIR *dir);
void			expand_command(t_env *env,
					t_token_array **tokens, size_t cursor);
int				is_expandable(t_token_type type);
#endif // ZEN_H
