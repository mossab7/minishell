#include <zen.h>

t_context **get_context()
{
    static t_context *context = NULL;

    if (context == NULL)
    {
        context = malloc(sizeof(t_context));
        context->state = 0;
        context->readline_active = 1;
    }
    return &context;
}
