/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 11:58:22 by rsanchez          #+#    #+#             */
/*   Updated: 2021/11/02 16:14:26 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void	exit_program(t_pipex *pipex, char *str, int size)
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
	exit(0);
}

static void	start_pipe(t_pipex *pipex, int ac, char **av)
{
	int		fd_in;

	if (str_n_comp(av[1], "here_doc\0", 9) == 0)
	{
		if (ac < 6)
			exit_program(pipex, "Wrong number of arguments\n", 26);
		pipex->av = &(av[2]);
		pipex->here_doc = TRUE;
		fd_in = redirect_infile(pipex);
		pipex->av = &(av[3]);
	}
	else if (str_n_comp(av[1], "/dev/urandom\0", 13) == 0)
		return ;
	else
	{
		pipex->av = &(av[2]);
		fd_in = open(av[1], O_RDONLY);
	}
	if (fd_in == -1)
		exit_program(pipex, "errno", -1);
	set_cmd_args(pipex, pipex->av[0]);
	through_pipe(pipex, fd_in);
}

/*
   if (!env)
   {
   extern char	**environ;
   env = environ;
   }*/

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;

	init_zero(&pipex, sizeof(pipex));
	if (ac < 5)
		exit_program(&pipex, "Wrong number of arguments\n", 26);
	set_env_paths(&pipex, env);
	pipex.env = env;
	start_pipe(&pipex, ac, av);
	exit_program(&pipex, NULL, -1);
	return (0);
}
