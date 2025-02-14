#include <ft_printf.h>

void	init_state(t_fstate *state, int fd, const char *fmt)
{
	ft_memset(state, 0, sizeof(*state));
	state->fmt = fmt;
	ft_io(fd);
}

void	clean_up_state(t_fstate *state)
{
	va_end(state->args);
	ft_io(STDOUT_FILENO);
}
