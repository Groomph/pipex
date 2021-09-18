/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:27:47 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/14 14:23:37 by rsanchez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*strjoin_free(char *str1, char *str2, BOOL freestr1, BOOL freestr2)
{
	size_t	i;
	size_t	i2;
	char	*temp;

	i = string_len(str1);
	i2 = string_len(str2);
	temp = malloc(sizeof(char) * (i + i2 + 1));
	if (!temp)
		return (NULL);
	temp[i + i2] = '\0';
	while (i2-- > 0)
		temp[i + i2] = str2[i2];
	while (i-- > 0)
		temp[i] = str1[i];
	if (str1 && freestr1)
		free(str1);
	if (str2 && freestr2)
		free(str2);
	return (temp);
}
