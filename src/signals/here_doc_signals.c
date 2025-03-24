#include <zen.h>

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;
	int					signals[] = {SIGABRT, SIGALRM, SIGBUS, SIGCHLD, SIGCONT,
							SIGFPE, SIGHUP, SIGILL, SIGPIPE, SIGPROF, SIGSEGV,
							SIGTERM, SIGTRAP, SIGTSTP, SIGTTIN, SIGTTOU,
							SIGUSR1, SIGUSR2, SIGVTALRM, SIGXCPU, SIGXFSZ};
	int					i;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	i = 0;
	while (i < (int)(sizeof(signals) / sizeof(signals[0])))
	{
		sigaction(signals[i], &sa, NULL);
		i++;
	}
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
