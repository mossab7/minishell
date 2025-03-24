/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/24 19:43:41 by mbouhia          ###   ########.fr       */
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
}				t_ctx_flags;

typedef struct s_context
{
	bool		siginit_received;
	int			readline_active;
	int			flags;
	t_string	*input;
	t_env		*env;
}				t_context;

t_context		**get_context(void);
t_string		*get_context_input(void);
void			init_context(t_string *initial_input, t_env *env);
t_env			*get_context_env(void);
void			toggle_context_flag(t_ctx_flags flag);
bool			check_context_flag(t_ctx_flags flag);
void			set_context_flag(t_ctx_flags flag);
void			unset_context_flag(t_ctx_flags flag);
#endif // !CONTEXT_H
