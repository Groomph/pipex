/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:28:01 by rsanchez          #+#    #+#             */
/*   Updated: 2020/12/24 19:44:49 by rsanchez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*substr_free(char *str, size_t start, size_t len, int freestr)
{
	size_t	i;
	char	*temp;

	i = string_len(str);
	if (!str || i < start)
	{
		temp = malloc(sizeof(char));
		if (!temp)
			return (NULL);
		temp[0] = '\0';
		return (temp);
	}
	if (i < start + len)
		len = i - start;
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
		return (NULL);
	i = -1;
	while (++i < len)
		temp[i] = str[start + i];
	temp[i] = '\0';
	if (str && freestr)
		free(str);
	return (temp);
}
