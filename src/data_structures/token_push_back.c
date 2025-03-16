/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_push_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:01:26 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 06:40:50 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	token_push_back(t_token	*tok, char c, u8 state)
{
	str_push_back(tok->lexeme, c);
	mask_push_back(tok->lexeme->mask, state);
}
