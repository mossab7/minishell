/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 01:24:19 by mbouhia           #+#    #+#             */
/*   Updated: 2025/04/20 21:00:40 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

void	handle_sigint(int signum)
{
	t_env	*env;

	(void)signum;
	env = get_context_env();
	env->last_command_status = EXIT_SIGINT;
	if (check_context_flag(FLAG_READLINE_ACTIVE))
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigquit(int signum)
{
	(void)signum;
}

void	signal_error(char *msg)
{
	perror(msg);
	cleanup_memory_tracker(get_memory_tracker());
	exit(1);
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGINT, &sa_int, 0) == -1)
		signal_error("sigaction (SIGINT)");
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGQUIT, &sa_quit, 0) == -1)
		signal_error("sigaction (SIGQUIT)");
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}
