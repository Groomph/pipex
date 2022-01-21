/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 11:58:22 by rsanchez          #+#    #+#             */
/*   Updated: 2022/01/21 22:59:28 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

void	exit_program(t_pipex *pipex, char *str, int size, int error)
{
	if (str)
	{
		if (size > 0)
			write(2, str, size);
		else
			perror(NULL);
	}
	if (pipex->paths)
		array_clear((void **)pipex->paths);
	if (pipex->path)
		free(pipex->path);
	if (pipex->args)
		array_clear((void **)pipex->args);
	if (pipex->fd_out != 0)
		close(pipex->fd_out);
	exit(error);
}

static int	wait_children(void)
{
	int	status;
	int	last;
	int	i;

	status = 0;
	i = 0;
	while (i != -1)
	{
		last = status;
		i = wait(&status);
	}
	if (WIFSIGNALED(last) == TRUE)
		return (WTERMSIG(last) + 128);
	return (WEXITSTATUS(last));
}

static void	start_pipe(t_pipex *pipex, int ac, char **av)
{
	int		fd_in;

	if (str_n_comp(av[1], "here_doc\0", 9) == 0)
	{
		if (ac < 6)
			exit_program(pipex, "Wrong number of args\n", 26, 1);
		pipex->av = &(av[2]);
		fd_in = redirect_infile(pipex, ac, av);
		pipex->av = &(av[3]);
	}
	else if (str_n_comp(av[1], "/dev/urandom\0", 13) == 0)
		return ;
	else
	{
		pipex->av = &(av[2]);
		fd_in = open_files(pipex, ac, av);
	}
	set_cmd_args(pipex, pipex->av[0]);
	through_pipe(pipex, fd_in);
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;

	init_zero(&pipex, sizeof(pipex));
	if (ac < 5)
		exit_program(&pipex, "Wrong number of arguments\n", 26, 1);
	set_env_paths(&pipex, env);
	pipex.env = env;
	start_pipe(&pipex, ac, av);
	exit_program(&pipex, NULL, -1, wait_children());
	return (0);
}
