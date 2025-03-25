/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 01:24:19 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/24 01:24:20 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

void	handle_sigint(int signum)
{
	(void)signum;
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

void	handle_sigterm(int signum)
{
	(void)signum;
	exit(0);
}

void	signal_error(char *msg)
{
	perror(msg);
	exit(1);
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	sa_term;

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
	sa_term.sa_handler = handle_sigterm;
	sigemptyset(&sa_term.sa_mask);
	sa_term.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGTERM, &sa_term, 0) == -1)
		signal_error("sigaction (SIGTERM)");
	signal(SIGTSTP, SIG_IGN);
}
