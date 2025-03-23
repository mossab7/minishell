/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_manipulation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:03:19 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/23 17:03:37 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	stack_push_back(t_stack *stack, int item)
{
	stack_expand(stack);
	stack->items[stack->size++] = item;
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

