#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <zen.h>

void handle_sigint(int signum)
{
    (void)signum;
    t_context *context = *get_context();

    if (context->readline_active == 1)
    {
		ft_putchar_fd('\n', STDOUT_FILENO);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
	}
}

void handle_sigquit(int signum)
{
    (void)signum;
}

void handle_sigterm(int signum)
{
    (void)signum;
    exit(0);
}

void setup_signal_handlers()
{
    struct sigaction sa_int, sa_quit, sa_term;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGINT, &sa_int, 0) == -1)
	{
        perror("sigaction (SIGINT)");
        exit(1);
    }

    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGQUIT, &sa_quit, 0) == -1)
	{
        perror("sigaction (SIGQUIT)");
        exit(1);
    }

    sa_term.sa_handler = handle_sigterm;
    sigemptyset(&sa_term.sa_mask);
    sa_term.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGTERM, &sa_term, 0) == -1)
	{
        perror("sigaction (SIGTERM)");
        exit(1);
    }
}
