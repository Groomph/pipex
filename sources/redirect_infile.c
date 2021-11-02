/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_infile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 00:49:29 by rsanchez          #+#    #+#             */
/*   Updated: 2021/11/02 16:10:50 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>

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
			exit_program(pipex, "Malloc error\n", 13);
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

int	redirect_infile(t_pipex *pipex)
{
	int	fd;

	fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd == -1)
		exit_program(pipex, "errno", -1);
	get_lines(pipex, fd, NULL, NULL);
	close(fd);
	fd = open(".here_doc", O_RDONLY);
	if (fd == -1)
	{
		if (unlink(".here_doc") == -1)
			exit_program(pipex, "errno", -1);
		exit_program(pipex, "errno", -1);
	}
	if (unlink(".here_doc") == -1)
		exit_program(pipex, "errno", -1);
	return (fd);
}
