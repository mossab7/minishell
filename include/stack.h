#ifndef STACK_H
#define STACK_H
#include <libft.h>
typedef struct s_stack
{
	int *items;
	size_t size;
	size_t cap;
}	t_stack;

void	stack_expand(t_stack *vec);
void	stack_push_back(t_stack *vec, int item);
t_stack	*stack_construct(void);
int		*stack_pop(t_stack *vec);
void	stack_destroy(t_stack *stack);
int		*stack_peek(t_stack *stack);
#endif // !STACK_H
