/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 12:05:27 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/18 19:55:09 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"

typedef struct s_pipex
{
	char	*paths;
	char	*infile;
	BOOL	here_doc;
	char	*limiter;
	int		limiter_size;
	t_list	*cmds;
	t_list	*last_c;
	t_list	*active;
	char	*outfile;
}		t_pipex;

enum	e_command
{
	ZZZ,
	OPEN,
	PATHS,
	MALLOC,
	DUP2,
	EXECVE,
	ARGS,
	FORK,
	PIPE,
	UNLINK
};

void	exit_program(t_pipex *pipex, int error, char *name, int size);
char	*find_env_paths(t_pipex *pipex, char **env);
char	*find_command_path(t_pipex *pipex, char *cmd, int size_c);
void	parse_args(t_pipex *pipex, int ac, char **av);
void	through_pipe(t_pipex *pipex, char **env, int fd_in);
int		redirect_infile(t_pipex *pipex);

#endif
