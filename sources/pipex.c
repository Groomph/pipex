/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 21:05:26 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/17 00:52:54 by rsanchez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void	child_work_end(t_pipex *pipex, char **env, int fd_in)
{
	int	fd_out;

	if (pipex->here_doc)
		fd_out = open(pipex->outfile, O_WRONLY | O_CREAT | O_APPEND,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	else
		fd_out = open(pipex->outfile, O_WRONLY | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd_out == -1)
	{
		close(fd_in);
		exit_program(pipex, OPEN, "child_work_end", 14);
	}
	if (dup2(fd_in, 0) == -1 || dup2(fd_out, 1) == -1)
	{
		close(fd_in);
		close(fd_out);
		exit_program(pipex, DUP2, "child_work_end", 14);
	}
	execve(pipex->active->path, pipex->active->args, env);
	close(fd_in);
	close(fd_out);
	exit_program(pipex, EXECVE, "child_work", 10);
}

static void	pipex_end(t_pipex *pipex, char **env, int fd_in)
{
	pid_t	pid;

	pid = fork();
	if (pid <= -1)
	{
		close(fd_in);
		exit_program(pipex, FORK, "pipex_end", 9);
	}
	else if (pid == 0)
	{
		child_work_end(pipex, env, fd_in);
	}
	else
	{
		wait(NULL);
		close(fd_in);
	}
}

void	child_work(t_pipex *pipex, char **env, int *fd, int fd_in)
{
	close(fd[0]);
	if (dup2(fd_in, 0) == -1 || dup2(fd[1], 1) == -1)
	{
		close(fd_in);
		close(fd[1]);
		exit_program(pipex, DUP2, "child_work", 10);
	}
	execve(pipex->active->path, pipex->active->args, env);
	close(fd_in);
	close(fd[1]);
	exit_program(pipex, EXECVE, "child_work", 10);
}

static void	parent_work(t_pipex *pipex, char **env, int *fd, int fd_in)
{
	wait(NULL);
	close(fd_in);
	close(fd[1]);
	pipex->active = pipex->active->next;
	if (pipex->active == pipex->last_c)
		pipex_end(pipex, env, fd[0]);
	else
		through_pipe(pipex, env, fd[0]);
}

void	through_pipe(t_pipex *pipex, char **env, int fd_in)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		close(fd_in);
		exit_program(pipex, PIPE, "through_pipe", 12);
	}
	pid = fork();
	if (pid <= -1)
	{
		close(fd[0]);
		close(fd[1]);
		close(fd_in);
		exit_program(pipex, FORK, "through_pipe", 12);
	}
	else if (pid == 0)
		child_work(pipex, env, fd, fd_in);
	else
		parent_work(pipex, env, fd, fd_in);
}
