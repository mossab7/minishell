/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:34:59 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 21:07:35 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

int	execute_here_doc(t_redirect *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	unlink(redir->filename);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	inptu_redirection(t_redirect *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (fd);
}

int	output_redirection(t_redirect *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (fd);
}

int	append_redirection(t_redirect *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (fd);
}

int	setup_redirections(t_command *cmd)
{
	t_redirect	*redir;
	int			fd;
	int			i;

	i = 0;
	while (i < cmd->redirect_count)
	{
		redir = cmd->redirects[i++];
		if (redir->type != REDIR_HEREDOC)
		{
			if (is_ambiguous_redirect(redir->filename))
				return ((zen_elog("ambiguous redirect\n")), -1);
		}
		if (redir->type == REDIR_INPUT)
			fd = inptu_redirection(redir);
		else if (redir->type == REDIR_OUTPUT)
			fd = output_redirection(redir);
		else if (redir->type == REDIR_APPEND)
			fd = append_redirection(redir);
		else if (redir->type == REDIR_HEREDOC)
			fd = execute_here_doc(redir);
		if (fd == -1)
			return (-1);
	}
	return (0);
}
