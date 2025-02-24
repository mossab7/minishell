#include <zen.h>

t_context **get_context()
{
	static t_context *context;

	return (&context);
}
