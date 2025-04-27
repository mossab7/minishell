/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:34:44 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 20:34:50 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

int	handle_pipe_error(int *pipefd, char *msg)
{
	perror(msg);
	if (pipefd)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (-1);
}

void	launch_left_pipe(t_ast *node, t_env *env, int pipefd[2])
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_ast(node->left, env));
}

void	launch_right_pipe(t_ast *node, t_env *env, int pipefd[2])
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_ast(node->right, env));
}

int	execute_pipe(t_ast *node, t_env *env)
{
	int		pipefd[2];
	int		pipe_status[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		return (handle_pipe_error(NULL, "pipe"));
	pid1 = fork();
	if (pid1 == -1)
		return (handle_pipe_error(pipefd, "fork"));
	if (pid1 == 0)
		launch_left_pipe(node, env, pipefd);
	pid2 = fork();
	if (pid2 == -1)
		return (handle_pipe_error(pipefd, "fork"));
	if (pid2 == 0)
		launch_right_pipe(node, env, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &pipe_status[0], 0);
	waitpid(pid2, &pipe_status[1], 0);
	if (WEXITSTATUS(pipe_status[0]) == EXIT_SIGINT
		|| WEXITSTATUS(pipe_status[1]) == EXIT_SIGINT)
		set_context_flag(FLAG_SIGINT_RECEIVED);
	return (env->last_command_status = WEXITSTATUS(pipe_status[1]));
}
