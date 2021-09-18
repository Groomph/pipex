/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_duplicate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romain <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 15:03:26 by romain            #+#    #+#             */
/*   Updated: 2021/09/15 15:09:45 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*string_duplicate(const char *original, int size)
{
	int		i;
	char	*new;

	i = 0;
	if (size <= 0)
		size = string_len(original);
	new = malloc(sizeof(char) * (size + 1));
	if (!new)
		return (NULL);
	while (original[i])
	{
		new[i] = original[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
