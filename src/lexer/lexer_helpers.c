/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 05:40:18 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 14:49:57 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	ft_is_operator(int c)
{
	return ((c == PIPE) || (c == AND) || (c == INPUT_REDIRECT)
		|| (c == OUTPUT_REDIRECT) || (c == OPAREN) || (c == CPAREN));
}

int	ft_zen_isalnum(int c)
{
	if (!c)
		return (0);
	return (!ft_isspace(c) && (ft_isalnum(c) || !ft_is_operator(c)));
}

int	is_inside_quotes(t_u8 state)
{
	return ((state & SINGLE_QUOTED) || (state & DOUBLE_QUOTED));
}

int	is_valid_word_character(int c, t_u8 state)
{
	if (!c)
		return (0);
	return (ft_zen_isalnum(c) || is_quote(c) || c == EXPANSION_MARK
		|| is_inside_quotes(state));
}
