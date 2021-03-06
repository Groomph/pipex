/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:43:54 by rsanchez          #+#    #+#             */
/*   Updated: 2021/10/14 19:53:59 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_array_size(const char *str, char c)
{
	int		i;
	int		check;
	int		count;

	if (!str)
		return (0);
	i = 0;
	check = 1;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			check++;
		else if (check)
		{
			check = 0;
			count++;
		}
		i++;
	}
	return (count);
}

static const char	*filltab(char **tmp, const char *str, char c)
{
	int		i;
	int		i2;

	while (*str == c)
		str++;
	i = 0;
	while (str[i] && str[i] != c)
		i++;
	*tmp = malloc(sizeof(char) * (i + 1));
	if (!(*tmp))
		return (NULL);
	i2 = 0;
	while (i2 < i)
	{
		(*tmp)[i2] = str[i2];
		i2++;
	}
	(*tmp)[i] = '\0';
	return (&str[i]);
}

char	**string_split(const char *str, char c)
{
	int		array_size;
	char	**tmp;
	int		i;

	array_size = count_array_size(str, c);
	tmp = malloc(sizeof(char *) * (array_size + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < array_size)
	{
		str = filltab(&(tmp[i]), str, c);
		if (!str)
		{
			i = 0;
			while (tmp[i])
				free(tmp[i++]);
			free(tmp);
			return (NULL);
		}
		i++;
	}
	tmp[array_size] = NULL;
	return (tmp);
}
