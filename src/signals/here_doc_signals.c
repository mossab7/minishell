/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:52:02 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/20 21:00:30 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	handle_child_sigint(int signo)
{
	(void)signo;
	exit(130);
}

void	setup_heredoc_signals(void)
{
	int	signo;

	signo = 1;
	while (signo < NSIG)
	{
		if (signo != SIGKILL && signo != SIGSTOP)
			signal(signo, SIG_IGN);
		signo++;
	}
	signal(SIGINT, handle_child_sigint);
}
