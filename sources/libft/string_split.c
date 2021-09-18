/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/14 13:43:54 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/18 19:38:30 by rsanchez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

BOOL	skip_quote(const char *str, char *quote_t)
{
	int		i;
	BOOL	is_closed;

	if (str[0] != *quote_t && *quote_t != '\0')
		return (FALSE);
	if (*quote_t == '\0')
	{
		i = 1;
		is_closed = FALSE;
		while (str[i] && !is_closed)
		{
			if (str[i] == str[0])
				is_closed = TRUE;
			i++;
		}
		if (!is_closed)
			return (FALSE);
		*quote_t = str[0];
	}
	else
		*quote_t = '\0';
	return (TRUE);
}

static int	count_array_size(const char *str, char c)
{
	int		i;
	int		check;
	int		count;
	char	quote_t;

	if (!str)
		return (0);
	i = 0;
	check = 1;
	count = 0;
	quote_t = '\0';
	while (str[i])
	{
		if (str[i] == 39 || str[i] == 34)
			skip_quote(&(str[i]), &quote_t);
		if (str[i] == c && quote_t == '\0')
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

static const char	*filltab2(char **tmp, const char *str, int max)
{
	int		i;
	int		i2;
	char	quote_t;

	i = 0;
	i2 = 0;
	quote_t = '\0';
	while (i + i2 < max)
	{
		if ((str[i + i2] != 39 && str[i + i2] != 34)
			|| !skip_quote(&(str[i + i2]), &quote_t))
		{
			(*tmp)[i] = str[i + i2];
			i++;
		}
		else
			i2++;
	}
	(*tmp)[i] = '\0';
	return (&str[i]);
}

static const char	*filltab(char **tmp, const char *str, char c)
{
	int		i;
	int		i2;
	char	quote_t;

	while (*str && *str == c)
		str++;
	i = 0;
	i2 = 0;
	quote_t = '\0';
	while (str[i] && (str[i] != c || quote_t != '\0'))
	{
		if ((str[i] != 39 && str[i] != 34)
			|| !skip_quote(&(str[i]), &quote_t))
			i2++;
		i++;
	}
	*tmp = malloc(sizeof(char) * (i2 + 1));
	if (!(*tmp))
		return (NULL);
	return (filltab2(tmp, str, i));
}

char	**string_split(char const *str, char c)
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
			return (NULL);
		}
		i++;
	}
	tmp[array_size] = NULL;
	return (tmp);
}
