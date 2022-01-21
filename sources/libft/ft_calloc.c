/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 19:09:26 by rsanchez          #+#    #+#             */
/*   Updated: 2022/01/21 19:14:45 by rsanchez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	*ft_calloc(size_t nb, size_t size)
{	
	void	*tmp;

	if (nb == 0 || size == 0)
		return (NULL);
	size *= nb;
	tmp = malloc(size);
	if (!tmp)
		return (NULL);
	init_zero(tmp, size);
	return (tmp);
}
