/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:40:11 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 06:40:13 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	stack_expand(t_stack *stack)
{
	if(stack->size >= stack->cap)
	{
		stack->cap *= VEC_GROWTH_FAC;
		stack->items = ft_realloc(stack->items,
					stack->cap * sizeof(*(stack->items)),
					stack->size * sizeof(*(stack->items)));
	}
}

void	stack_push_back(t_stack *stack, int item)
{
	stack_expand(stack);
	stack->items[stack->size++] = item;
}

t_stack	*stack_construct(void)
{
	t_stack	*stack;

	stack = alloc(sizeof(*stack));
	stack->items = alloc(VAC_INIT_CAP * sizeof(*(stack->items)));
	stack->cap = VEC_INIT_CAP;
	stack->size = 0;
	return (stack);
}

int	*stack_pop(t_stack *stack)
{
	if (stack && stack->size)
		return (&stack->items[--stack->size]);
	return (NULL);
}

int	*stack_peek(t_stack *stack)
{
	if (stack && stack->size)
		return (&stack->items[-1 + stack->size]);
	return (NULL);
}

void	stack_destroy(t_stack *stack)
{
	if (stack)
	{
		if (stack->items)
			ft_free(stack->items);
		ft_free(stack);
	}
}
