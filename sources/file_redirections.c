/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 00:49:29 by rsanchez          #+#    #+#             */
/*   Updated: 2022/01/21 23:00:05 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void	display_bash_error(t_pipex *pipex, char *file, BOOL quit)
{
	write(2, "bash: ", 6);
	write(2, file, string_len(file));
	write(2, ": ", 2);
	if (quit)
		exit_program(pipex, "errno", -1, 1);
	else
		perror(NULL);
}

int	open_files(t_pipex *pipex, int ac, char **av)
{
	int	fd_in;

	fd_in = open(av[1], O_RDONLY);
	if (fd_in == -1)
	{
		display_bash_error(pipex, av[1], FALSE);
		fd_in = open(".poubelleosef", O_RDONLY | O_CREAT);
		if (fd_in == -1)
			exit_program(pipex, "errno", -1, 1);
		if (unlink(".poubelleosef") == -1)
		{
			close(fd_in);
			exit_program(pipex, "errno", -1, 1);
		}
	}
	ac--;
	pipex->fd_out = open(av[ac], O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (pipex->fd_out == -1)
	{
		close(fd_in);
		display_bash_error(pipex, av[ac], TRUE);
	}
	return (fd_in);
}

static void	get_lines(t_pipex *pipex, int fd, char *buffer, char *line)
{
	BOOL	eof;
	int		size;

	eof = FALSE;
	while (!eof)
	{
		write(1, "> ", 2);
		size = get_next_line(0, &buffer, &line, &eof);
		if (size == -1)
		{
			free(buffer);
			free(line);
			exit_program(pipex, "Malloc error\n", 13, 1);
		}
		if (str_n_comp(line, pipex->av[0], string_len(pipex->av[0]) + 1) == 0)
			eof = TRUE;
		else
		{
			write(fd, line, size);
			write(fd, "\n", 1);
		}
		free(line);
	}
	free(buffer);
}

int	redirect_infile(t_pipex *pipex, int ac, char **av)
{
	int	fd;

	fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd == -1)
		exit_program(pipex, "errno", -1, 1);
	get_lines(pipex, fd, NULL, NULL);
	close(fd);
	fd = open(".here_doc", O_RDONLY);
	if (fd == -1)
	{
		unlink(".here_doc");
		exit_program(pipex, "errno", -1, 1);
	}
	if (unlink(".here_doc") == -1)
		exit_program(pipex, "errno", -1, 1);
	ac--;
	pipex->fd_out = open(av[ac], O_WRONLY | O_CREAT | O_APPEND,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (pipex->fd_out == -1)
	{
		close(fd);
		display_bash_error(pipex, av[ac], TRUE);
	}
	return (fd);
}
