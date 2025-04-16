/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zen.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 06:38:43 by lazmoud           #+#    #+#             */
/*   Updated: 2025/04/16 22:19:36 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

int	main(int ac, char **av, const char *envp[])
{
	t_parser	parser;

	(void)ac;
	initialize_shell(&parser, envp, *av);
	shell_loop(&parser);
	cleanup_memory_tracker(get_memory_tracker());
	return (0);
}
