/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:45:34 by mbouhia           #+#    #+#             */
/*   Updated: 2025/04/27 17:17:17 by lazmoud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

int	init_heredoc(t_string **filename, int *fd, int pipefd[2])
{
	*filename = vstr_construct(1, ft_mkstemp());
	*fd = open((*filename)->cstring, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (*fd == -1)
	{
		perror("heredoc");
		return (-1);
	}
	return (0);
}

t_string	*read_heredoc_content(t_redirect *redir, int fd)
{
	t_string	*line;
	t_string	*input;

	input = str_construct();
	jump_dollar_signs(redir->heredoc_delimiter);
	while (true)
	{
		line = ft_readline("> ");
		if (line == NULL)
		{
			zen_elog(DL, redir->heredoc_delimiter->cstring);
			break ;
		}
		if (delimiter_reached(redir->heredoc_delimiter, line))
			break ;
		write(fd, line->cstring, line->size);
		write(fd, "\n", 1);
		str_join(input, 2, line->cstring, "\n");
		add_history(line->cstring);
		str_destruct(line);
	}
	return (input);
}

void	handle_heredoc_child(int fd, int pipefd[2], t_redirect *redir)
{
	t_string	*input;

	setup_heredoc_signals();
	close(pipefd[0]);
	input = read_heredoc_content(redir, fd);
	write(pipefd[1], input->cstring, input->size);
	close(pipefd[1]);
	close(fd);
	str_destruct(input);
	safe_exit(0);
}

void	read_from_pipe(int pipefd)
{
	char		*heredoc_content;
	t_string	*input;

	input = get_context_input();
	str_append("\n", input);
	while (true)
	{
		heredoc_content = get_next_line(pipefd);
		if (!heredoc_content)
			break ;
		str_append(heredoc_content, input);
		ft_free(heredoc_content);
	}
}

int	setup_here_doc(t_redirect *redir)
{
	int		fd;
	pid_t	pid;
	int		status;
	int		pipefd[2];

	status = 0;
	set_context_flag(FLAG_HERE_DOC_ACTIVE);
	if (init_heredoc(&redir->filename, &fd, pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (cleanup_on_error(redir->filename->cstring, fd, -1));
	}
	if (pid == 0)
		handle_heredoc_child(fd, pipefd, redir);
	close(fd);
	close(pipefd[1]);
	read_from_pipe(pipefd[0]);
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(WEXITSTATUS(status)))
		return (cleanup_on_error(redir->filename->cstring, -1, status));
	return (0);
}
