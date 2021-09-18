/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 21:05:46 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/18 15:54:31 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

void	print_all(t_pipex *pipex)
{
	t_list	*tmp;
	int		i;

	printf("infile: %s\n", pipex->infile);
	tmp = pipex->cmds;
	while (tmp)
	{
		printf("cmd: %s args: ", tmp->path);
		i = 0;
		while (tmp->args[i])
		{
			printf("%i: %s ", i, tmp->args[i]);
			i++;
		}
		printf("\n");
		tmp = tmp->next;
	}
	printf("outfile: %s\n", pipex->outfile);
	printf("paths: %s\n", pipex->paths);
}

static void	parse_cmd(t_pipex *pipex, t_list *new_cmd, char *av, int size_c)
{
	int	i;

	new_cmd->args = string_split(av, ' ');
	if (!(new_cmd->args))
		exit_program(pipex, MALLOC, "parse_cmd", 9);
	i = 0;
	while (av[i] && !is_whitespace(av[i]))
		i++;
	if (size_c == -1 && av[i])
	{
		av[i] = '\0';
		i++;
	}
	new_cmd->path = find_command_path(pipex, av, size_c);
}

static void	create_new_list(t_pipex *pipex, char *av, int size_cmd)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		exit_program(pipex, MALLOC, "create_new_list", 15);
	lst_addback(&(pipex->cmds), new, pipex->last_c);
	pipex->last_c = new;
	parse_cmd(pipex, new, av, size_cmd);
}

void	parse_args(t_pipex *pipex, int ac, char **av)
{
	int		i;

	if (ac >= 5)
	{
		pipex->infile = av[1];
		i = 2;
		if (str_n_comp(pipex->infile, "here_doc\0", 9) == 0)
		{
			if (ac < 6)
				exit_program(pipex, ARGS, NULL, 0);
			pipex->here_doc = TRUE;
			pipex->limiter = av[2];
			pipex->limiter_size = string_len(pipex->limiter + 1);
			i++;
		}
		while (i < ac - 1)
		{
			create_new_list(pipex, av[i], -1);
			i++;
		}
		pipex->outfile = av[i];
	}
	else
		exit_program(pipex, ARGS, NULL, 0);
	print_all(pipex);
}
