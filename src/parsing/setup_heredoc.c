/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 19:45:34 by mbouhia           #+#    #+#             */
/*   Updated: 2025/03/25 19:45:35 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <zen.h>

int	init_heredoc(char **filename, int *fd, int pipefd[2])
{
	*filename = ft_mkstemp();
	*fd = open(*filename, O_RDWR | O_CREAT | O_EXCL, 0600);
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
	while (true)
	{
		line = ft_readline("> ");
		if (line == NULL)
		{
			zen_elog(DL, redir->delimiter);
			break ;
		}
		if (strcmp(line->cstring, redir->delimiter) == 0)
		{
			str_destruct(line);
			break ;
		}
		string_expand(get_context_env(), line);
		write(fd, line->cstring, line->size);
		write(fd, "\n", 1);
		str_join(input, 2, line->cstring, "\n");
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
	exit(0);
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
		return (cleanup_on_error(redir->filename, fd, -1));
	}
	if (pid == 0)
		handle_heredoc_child(fd, pipefd, redir);
	close(fd);
	close(pipefd[1]);
	read_from_pipe(pipefd[0]);
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(WEXITSTATUS(status)))
		return (cleanup_on_error(redir->filename, -1, status));
	return (0);
}

int heredoc_collect_all(t_ast *root)
{
	pid_t	pid;
	int		pipefd[2];
	int		status;

	pid = fork();
	status = 0;
	if (pipe(pipefd) == -1)
		return ((perror("pipe")),-1);
	if (pid == -1)
		return ((perror("fork")),-1);
	if(pid == 0)
	{
		setup_heredoc_signals();
		close(pipefd[0]);
		heredoc_traverse_ast(root,pipefd);
		close(pipefd[1]);
		return (0);
	}
	close(pipefd[1]);
	read_from_pipe(pipefd[0]);
	close(pipefd[0]);
	waitpid(pid,&status,0);
	if (WIFSIGNALED(WEXITSTATUS(status)))
		return (printf("\n"),set_context_flag(FLAG_SYNTAX_ERROR),1);
	return (0);
}

void	heredoc_traverse_ast(t_ast *root, int *pipefd)
{
	int		i;

	if (root == NULL)
		return ;
	heredoc_traverse_ast(root->left,pipefd);
	heredoc_traverse_ast(root->right,pipefd);
	if(root->type == NODE_COMMAND)
	{
		i = 0;
		while(i < root->u_value.command.redirect_count)
		{
			if (root->u_value.command.redirects[i]->type == REDIR_HEREDOC)
				setup_here_doc(root->u_value.command.redirects[i]);
			i++;
		}
	}
}
