/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_push_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:01:26 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/05 13:10:00 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

void	token_push_back(t_token	*tok, char c, u8 state)
{
	str_push_back(tok->lexeme, c);
	// NOTE: This mask has info about wether the current added character is inside double or single quotes.
	// so I can just use if (tok->mask->items[index] == DOUBLE_QOUTE) it is inside dooubles.
	// else if if (tok->mask->items[index] == SIGNE_QOUTES) it is inside single qutes.
	mask_push_back(tok->mask, state);
}
