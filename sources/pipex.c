/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 21:05:26 by rsanchez          #+#    #+#             */
/*   Updated: 2022/01/21 22:57:43 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

static void	closefd_quit(t_pipex *pipex, int read, int write, int fd_in)
{
	if (read != -1)
		close(read);
	if (write != -1)
		close(write);
	if (fd_in != -1)
		close(fd_in);
	exit_program(pipex, "errno", -1, 1);
}

static void	child_work(t_pipex *pipex, int read, int writ, int fd_in)
{
	if (read != -1)
		close(read);
	if (dup2(fd_in, 0) == -1 || dup2(writ, 1) == -1)
		closefd_quit(pipex, -1, writ, fd_in);
	close(fd_in);
	close(writ);
	execve(pipex->path, pipex->args, pipex->env);
	write(2, "bash: ", 6);
	write(2, pipex->args[0], string_len(pipex->args[0]));
	exit_program(pipex, ": command not found\n", 20, 127);
}

static void	pipex_end(t_pipex *pipex, int fd_in)
{
	pid_t	pid;

	pid = fork();
	if (pid <= -1)
		closefd_quit(pipex, -1, -1, fd_in);
	else if (pid == 0)
	{
		child_work(pipex, -1, pipex->fd_out, fd_in);
	}
	else
	{
		close(fd_in);
	}
}

void	through_pipe(t_pipex *pipex, int fd_in)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		closefd_quit(pipex, -1, -1, fd_in);
	pid = fork();
	if (pid <= -1)
		closefd_quit(pipex, fd[0], fd[1], fd_in);
	else if (pid == 0)
		child_work(pipex, fd[0], fd[1], fd_in);
	close(fd_in);
	close(fd[1]);
	pipex->av++;
	set_cmd_args(pipex, pipex->av[0]);
	if (!pipex->av[2])
		pipex_end(pipex, fd[0]);
	else
		through_pipe(pipex, fd[0]);
}
/*
   static void	parent_work(t_pipex *pipex, int *fd, int fd_in)//, pid_t pid)
   {
   close(fd_in);
   close(fd[1]);
//	*(pipex->cursor_pid) = pid;
//	(pipex->cursor_pid)++;
pipex->av++;
set_cmd_args(pipex, pipex->av[0]);
if (!pipex->av[2])
pipex_end(pipex, fd[0]);
else
through_pipe(pipex, fd[0]);
}
*/
