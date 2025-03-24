/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zen_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 14:28:09 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/16 14:28:44 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <zen.h>

t_string	*zen_prompt(t_env *env)
{
	char		*user;
	char		*pwd;
	t_string	*buff;
	t_string	*zen_prompt_;

	zen_prompt_ = str_construct();
	pwd = env_get(env, "PWD");
	if (!pwd)
		pwd = "PWD_NOT_SET";
	user = env_get(env, "USER");
	if (!user)
		user = "incognito";
	if (isatty(STDIN_FILENO))
		str_join(zen_prompt_, 4, user, "@", pwd, ": ");
	else
		zen_prompt_->cstring = NULL;
	set_context_flag(FLAG_READLINE_ACTIVE);
	if (check_context_flag(FLAG_SIGINT_RECEIVED))
		printf("\n");
	buff = ft_readline(zen_prompt_->cstring);
	str_destruct(zen_prompt_);
	return (buff);
}
