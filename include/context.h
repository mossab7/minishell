#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct s_context
{
    bool siginit_received;
    int readline_active;
} t_context;

t_context **get_context();

#endif // !CONTEXT_H
