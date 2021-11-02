/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 21:05:26 by rsanchez          #+#    #+#             */
/*   Updated: 2021/11/02 15:59:50 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void	child_work_end(t_pipex *pipex, int fd_in)
{
	int	fd_out;

	if (pipex->here_doc)
		fd_out = open(pipex->av[1], O_WRONLY | O_CREAT | O_APPEND,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	else
		fd_out = open(pipex->av[1], O_WRONLY | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd_out == -1)
	{
		close(fd_in);
		exit_program(pipex, "errno", -1);
	}
	else if (dup2(fd_in, 0) == -1 || dup2(fd_out, 1) == -1)
	{
		close(fd_in);
		close(fd_out);
		exit_program(pipex, "errno", -1);
	}
	close(fd_in);
	close(fd_out);
	execve(pipex->path, pipex->args, pipex->env);
	exit_program(pipex, "command not found\n", 18);
}

static void	pipex_end(t_pipex *pipex, int fd_in)
{
	pid_t	pid;

	pid = fork();
	if (pid <= -1)
	{
		close(fd_in);
		exit_program(pipex, "errno", -1);
	}
	else if (pid == 0)
	{
		child_work_end(pipex, fd_in);
	}
	else
	{
		wait(NULL);
		close(fd_in);
	}
}

void	child_work(t_pipex *pipex, int *fd, int fd_in)
{
	close(fd[0]);
	if (dup2(fd_in, 0) == -1 || dup2(fd[1], 1) == -1)
	{
		close(fd_in);
		close(fd[1]);
		exit_program(pipex, "errno", -1);
	}
	close(fd_in);
	close(fd[1]);
	execve(pipex->path, pipex->args, pipex->env);
	exit_program(pipex, "command not found\n", 18);
}

static void	parent_work(t_pipex *pipex, int *fd, int fd_in)
{
	int	statut;

	wait(&statut);
	close(fd_in);
	close(fd[1]);
	pipex->av++;
	set_cmd_args(pipex, pipex->av[0]);
	if (!pipex->av[2])
		pipex_end(pipex, fd[0]);
	else
		through_pipe(pipex, fd[0]);
}

void	through_pipe(t_pipex *pipex, int fd_in)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		close(fd_in);
		exit_program(pipex, "errno", -1);
	}
	pid = fork();
	if (pid <= -1)
	{
		close(fd[0]);
		close(fd[1]);
		close(fd_in);
		exit_program(pipex, "errno", -1);
	}
	else if (pid == 0)
		child_work(pipex, fd, fd_in);
	else
		parent_work(pipex, fd, fd_in);
}
