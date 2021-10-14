/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 21:05:46 by rsanchez          #+#    #+#             */
/*   Updated: 2021/10/14 19:59:05 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdio.h>

void	set_env_paths(t_pipex *pipex, char **env)
{
	int	i;

	i = 0;
	while (env[i] && str_n_comp(env[i], "PATH=", 5) != 0)
		i++;
	pipex->paths = string_split(&(env[i][5]), ':');
	if (!(pipex->paths))
		exit_program(pipex, MALLOC, NULL, 0);
}

static char	*build_path(char *paths, char *cmd, int size_p, int size_c)
{
	int		i;
	int		i2;
	char	*new_path;

	new_path = malloc(sizeof(char) * (size_p + size_c + 2));
	if (!new_path)
		return (NULL);
	i = 0;
	while (paths[i] && paths[i] != ':')
	{
		new_path[i] = paths[i];
		i++;
	}
	new_path[i++] = '/';
	i2 = 0;
	while (i2 < size_c)
	{
		new_path[i + i2] = cmd[i2];
		i2++;
	}
	new_path[i + i2] = '\0';
	return (new_path);
}

static char	*find_command_path(t_pipex *pipex, char *cmd, int size_cmd)
{
	int		i;
	int		size_path;
	char	*new_path;

	i = 0;
	while (pipex->paths[i])
	{
		size_path = string_len(pipex->paths[i]);
		new_path = build_path(pipex->paths[i], cmd, size_path, size_cmd);
		if (!new_path)
			exit_program(pipex, MALLOC, "build_path", 10);
		if (access(new_path, X_OK) == 0)
			return (new_path);
		else
			free(new_path);
		i++;
	}
	new_path = string_duplicate(cmd, size_cmd);
	if (!new_path)
		exit_program(pipex, MALLOC, "build_path2", 11);
	return (new_path);
}

/*	if (access(pipex->path, X_OK) == -1)
	{
		free(pipex->path);
		free(pipex->args);
		exit_program(pipex, PATH, av, i);
	}
*/

void	set_cmd_args(t_pipex *pipex, char *av)
{
	int	i;

	if (pipex->path)
		free(pipex->path);
	if (pipex->args)
		array_clear((void **)pipex->args);
	pipex->args = quote_handler(av, ' ');
	if (!(pipex->args))
		exit_program(pipex, MALLOC, "parse_cmd", 9);
	i = 0;
	while (av[i] && !is_whitespace(av[i]))
		i++;
	if (av[i])
		av[i] = '\0';
	pipex->path = find_command_path(pipex, av, i);
}
