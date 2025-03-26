/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:43:41 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/24 19:43:41 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define EXIT_SIGINT 130

void	setup_signal_handlers(void);
void	setup_heredoc_signals(void);
#endif
