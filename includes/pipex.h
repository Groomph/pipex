/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 12:05:27 by rsanchez          #+#    #+#             */
/*   Updated: 2022/01/21 23:00:36 by rsanchez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"

typedef struct s_pipex
{
	char	**paths;
	char	**av;
	char	**env;
	char	*path;
	char	**args;
	int		fd_out;
}		t_pipex;

void	exit_program(t_pipex *pipex, char *str, int size, int error);
void	set_env_paths(t_pipex *pipex, char **env);
void	set_cmd_args(t_pipex *pipex, char *av);
void	through_pipe(t_pipex *pipex, int fd_in);
int		open_files(t_pipex *pipex, int ac, char **av);
int		redirect_infile(t_pipex *pipex, int ac, char **av);
char	**quote_handler(char *str, char c);

#endif
