/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/24 19:43:41 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H
# include <fcntl.h>
# include <syntax_tree.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <zen.h>

int		execute_ast(t_ast *node, t_env *env);
int		execute_here_doc(t_redirect *redir);
int		inptu_redirection(t_redirect *redir);
int		output_redirection(t_redirect *redir);
int		append_redirection(t_redirect *redir);
int		setup_redirections(t_command *cmd);
int		handle_pipe_error(int *pipefd, char *msg);
void	launch_left_pipe(t_ast *node, t_env *env, int pipefd[2]);
void	launch_right_pipe(t_ast *node, t_env *env, int pipefd[2]);
int		execute_pipe(t_ast *node, t_env *env);
bool	is_ambiguous_redirect(const char *filename);
int		execute_subshell(t_ast *node, t_env *env);
int		execute_logical_or(t_ast *node, t_env *env, int left_status);
void	reset_redirections(int *original_stdin, int *original_stdout);
int		setup_builtin_redirections(t_command *cmd, t_type type);
void	init_builtin_commands(t_built_in_command *functions,
			char **function_names);
int		execute_built_in_commands(t_command *cmd, char *command, t_env *env,
			char **args);
int		get_command_status(t_env *env, int status);
int		execute_command(t_command *cmd, t_env *env);
void	launch_command(t_command *cmd, t_env *env, char **args);
#endif // EXECUTE_H
