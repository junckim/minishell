/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 18:21:43 by joockim           #+#    #+#             */
/*   Updated: 2020/04/09 18:21:59 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list *res;

	if (new == 0 || lst == 0)
		return ;
	if (*lst == 0)
	{
		*lst = new;
		return ;
	}
	res = ft_lstlast(*lst);
	res->next = new;
}
