/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 11:58:22 by rsanchez          #+#    #+#             */
/*   Updated: 2021/10/11 16:38:18 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

static const int	g_i_e[11] =
{
	3,
	14,
	27,
	0
};

static const char	*g_str_error[11] =
{
	"ZZZ",
	"malloc error: ",
	"Too few arguments for pipex",
	NULL
};

/*
static void	free_cmds(t_list cmd)
{
	if (cmd->next)
		free_cmds(cmd->next);
	array_clear(cmd->args);
	free(cmd->path);
	free(cmd);
}
*/

void	exit_program(t_pipex *pipex, int error, char *name, int size)
{
	if (error)
	{
		if (error == PATH)
		{
			write(2, "bash: ", 6);
			write(2, name, size);
			write(2, ": command not found\n", 20);
		}
		else if (error >= MALLOC)
		{
			write(2, g_str_error[error], g_i_e[error]);
			write(2, name, size);
			write(2, "\n", 1);
		}
		else
			perror(name);
	}
	array_clear((void **)pipex->paths);
	exit(0);
}

/*
	char *newviron[] = { NULL };
	extern char	**environ;
*/
static void	start_pipe(t_pipex *pipex, int ac, char **av)
{
	int		fd_in;

	if (ac < 5)
		exit_program(pipex, ARGS, NULL, 0);
	if (str_n_comp(av[1], "here_doc\0", 9) == 0)
	{
		if (ac < 6)
			exit_program(pipex, ARGS, NULL, 0);
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
		exit_program(pipex, ERRNO, NULL, 0);
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
	set_env_paths(&pipex, env);
	pipex.env = env;
	start_pipe(&pipex, ac, av);
	array_clear((void **)pipex.paths);
	return (0);
}
