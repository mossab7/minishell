#include <zen.h>

void	set_context_flag(t_ctx_flags flag)
{
	t_context	*context;

	context = *get_context();
	context->flags |= flag;
}

void	unset_context_flag(t_ctx_flags flag)
{
	t_context	*context;

	context = *get_context();
	context->flags &= ~flag;
}

void	toggle_context_flag(t_ctx_flags flag)
{
	t_context	*context;

	context = *get_context();
	context->flags ^= flag;
}

bool	check_context_flag(t_ctx_flags flag)
{
	t_context	*context;

	context = *get_context();
	return (context->flags & flag);
}
