/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_addback.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:55:40 by rsanchez          #+#    #+#             */
/*   Updated: 2021/09/15 15:09:29 by romain           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	lst_addback2(t_list **alst, t_list *new)
{
	if (!*alst)
		*alst = new;
	else if ((*alst)->next)
		lst_addback2(&(*alst)->next, new);
	else
		(*alst)->next = new;
}

void	lst_addback(t_list **alst, t_list *new, t_list *last)
{
	new->next = NULL;
	if (last)
		last->next = new;
	else
		lst_addback2(alst, new);
}
