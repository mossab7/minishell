/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/04/28 14:28:13 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include <stdbool.h>

typedef enum e_flags
{
	FLAG_SIGINT_RECEIVED = 1 << 0,
	FLAG_READLINE_ACTIVE = 1 << 1,
	FLAG_SYNTAX_ERROR = 1 << 2,
	FLAG_HERE_DOC_ACTIVE = 1 << 3,
	IN_SUBSHELL = 1 << 4
}				t_ctx_flags;

typedef struct s_context
{
	int			flags;
	t_string	*input;
	t_env		*env;
	t_env		*subshell_env;
	char		*pwd;
	int			_id_quoting;
}				t_context;

t_context	**get_context(void);
t_string	*get_context_input(void);
void		init_context(t_string *initial_input, t_env *env);
t_env		*get_context_env(void);
void		toggle_context_flag(t_ctx_flags flag);
bool		check_context_flag(t_ctx_flags flag);
void		set_context_flag(t_ctx_flags flag);
void		unset_context_flag(t_ctx_flags flag);
void		set_pwd(char *pwd);
void		set_subshell_env(t_env *senv);
void		unset_subshell_env(void);
int			get_is_insubshell(void);
int			*get_qid(void);
#endif // !CONTEXT_H
