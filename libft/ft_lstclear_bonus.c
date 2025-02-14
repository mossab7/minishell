/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:09:17 by lazmoud           #+#    #+#             */
/*   Updated: 2024/10/25 21:09:21 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*curr;
	t_list	*next;

	if (!*lst)
		return ;
	curr = *lst;
	next = curr->next;
	while (curr)
	{
		del(curr->content);
		free(curr);
		curr = next;
		if (curr)
			next = curr->next;
		else
			break ;
	}
	*lst = NULL;
}
