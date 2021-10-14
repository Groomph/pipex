/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 12:05:27 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/19 02:27:29 by rsanchez         ###   ########.fr       */
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
	BOOL	here_doc;
}		t_pipex;

enum	e_command
{
	ZZZ,
	ERRNO,
	PATH,
	MALLOC,
	ARGS
};

void	exit_program(t_pipex *pipex, int error, char *name, int size);
void	set_env_paths(t_pipex *pipex, char **env);
void	set_cmd_args(t_pipex *pipex, char *av);
void	through_pipe(t_pipex *pipex, int fd_in);
int		redirect_infile(t_pipex *pipex);
char	**quote_handler(char *str, char c);

#endif
