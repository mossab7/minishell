#include <zen.h>
#include <signal.h>

void	handle_child_sigint(int signo)
{
	(void)signo;
	exit(0);
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