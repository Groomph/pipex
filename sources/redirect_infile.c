/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_infile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 00:49:29 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/18 16:11:54 by rsanchez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
			exit_program(pipex, MALLOC, "redirect_infile", 15);
		}
		if (str_n_comp(line, pipex->limiter, pipex->limiter_size) == 0)
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
		exit_program(pipex, OPEN, "redirect_infile", 15);
	get_lines(pipex, fd, NULL, NULL);
	close(fd);
	fd = open(".here_doc", O_RDONLY);
	if (fd == -1)
	{
		if (unlink(".here_doc") == -1)
			exit_program(pipex, UNLINK, "redirect_infile", 15);
		exit_program(pipex, OPEN, "redirect_infile2", 16);
	}
	if (unlink(".here_doc") == -1)
		exit_program(pipex, UNLINK, "redirect_infile2", 16);
	return (fd);
}
