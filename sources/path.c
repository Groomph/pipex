/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 15:23:20 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/16 19:37:54 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>

#include <stdio.h>

char	*find_env_paths(t_pipex *pipex, char **env)
{
	int	i;

	i = 0;
	while (env[i] && str_n_comp(env[i], "PATH=", 5) != 0)
		i++;
	if (!(env[i]))
		exit_program(pipex, PATHS, NULL, 0);
	return (&(env[i][5]));
}

static int	find_size_path(char *paths)
{
	int	i;

	i = 0;
	while (paths[i] && paths[i] != ':')
		i++;
	return (i);
}

static char	*build_path(char **paths, char *cmd, int size_p, int size_c)
{
	int		i;
	int		i2;
	char	*new_path;

	new_path = malloc(sizeof(char) * (size_p + size_c + 2));
	if (!new_path)
		return (NULL);
	i = 0;
	while ((*paths)[i] && (*paths)[i] != ':')
	{
		new_path[i] = (*paths)[i];
		i++;
	}
	*paths = &((*paths)[i]);
	if (**paths == ':')
		(*paths)++;
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

char	*find_command_path(t_pipex *pipex, char *cmd, int size_cmd)
{
	char	*paths;
	int		size_path;
	char	*new_path;

	if (size_cmd < 0)
		size_cmd = string_len(cmd);
	paths = pipex->paths;
	while (*paths)
	{
		size_path = find_size_path(paths);
		new_path = build_path(&paths, cmd, size_path, size_cmd);
		if (!new_path)
			exit_program(pipex, MALLOC, "build_path", 10);
		if (access(new_path, X_OK) == 0)
			return (new_path);
		else
			free(new_path);
	}
	new_path = string_duplicate(cmd, size_cmd);
	if (!new_path)
		exit_program(pipex, MALLOC, "build_path2", 11);
	return (new_path);
}

/*
char    *find_command_path(t_pipex *pipex, char *cmd, char **env)
{
	char	**paths_array;
	int	i;
	int	i2;
	char	*path;
	int	size_command;

	size_command = string_len(cmd);
	paths_array = string_split(pipex->paths, ':');
	i = 0;
	while (paths_array[i])
	{
		paths_array[i] = strjoin_free(paths_array[i], "/", TRUE, FALSE);
		paths_array[i] = strjoin_free(paths_array[i], cmd, TRUE, FALSE);
		if (access(paths_array[i], X_OK) == 0)
		{
		i2 = 1;
		path = paths_array[i];
		while (paths_array[i + i2])
		{
			free(paths_array[i + i2]);
			i2++;
		}
		free(paths_array);
		return (path);
		}
		else
			free(paths_array[i]);
		i++;
	}
	free(paths_array);
	return (NULL);
	}
*/
