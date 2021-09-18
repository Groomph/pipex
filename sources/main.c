/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 11:58:22 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/18 16:08:33 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static const int	g_i_e[11] =
{
	3,
	21,
	49,
	14,
	6,
	14,
	27,
	16,
	12,
	14,
	0
};

static const char	*g_str_error[11] =
{
	"ZZZ",
	"Unable to open file: ",
	"Unable to find path WTF!!! Not supposed to happen",
	"malloc error: ",
	"dup2: ",
	"execve error: ",
	"Too few arguments for pipex",
	"Unable to fork: ",
	"pipe error: ",
	"unlink error: ",
	NULL
};

void	exit_program(t_pipex *pipex, int error, char *name, int size)
{
	t_list	*tmp;
	int		i;

	if (error)
	{
		write(2, g_str_error[error], g_i_e[error]);
		if (name)
			write(2, name, size);
		write(2, "\n", 1);
	}
	while (pipex->cmds)
	{
		i = 0;
		while (pipex->cmds->args[i])
		{
			free(pipex->cmds->args[i]);
			i++;
		}
		tmp = pipex->cmds->next;
		free(pipex->cmds->path);
		free(pipex->cmds->args);
		free(pipex->cmds);
		pipex->cmds = tmp;
	}
	exit(1);
}

/*
	char *newviron[] = { NULL };
	extern char	**environ;
*/

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;
	int		fd_in;

	init_zero(&pipex, sizeof(pipex));
	pipex.paths = find_env_paths(&pipex, env);
	parse_args(&pipex, ac, av);
	pipex.active = pipex.cmds;
	if (pipex.here_doc)
		fd_in = redirect_infile(&pipex);
	else
		fd_in = open(pipex.infile, O_RDONLY);
	if (fd_in == -1)
		exit_program(&pipex, OPEN, "main", 4);
	through_pipe(&pipex, env, fd_in);
	exit_program(&pipex, 0, NULL, 0);
	return (1);
}
