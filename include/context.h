#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct s_context
{
    int state;
    int readline_active;
} t_context;

t_context **get_context();

#endif // !CONTEXT_H
