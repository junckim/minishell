/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 18:00:25 by joockim           #+#    #+#             */
/*   Updated: 2020/04/17 22:30:54 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list *res;
	t_list *new;

	if (lst == 0 || f == 0)
		return (0);
	if (!(new = ft_lstnew(f(lst->content))))
	{
		ft_lstclear(&lst, del);
		return (0);
	}
	res = new;
	lst = lst->next;
	while (lst)
	{
		if (!(new = ft_lstnew(f(lst->content))))
		{
			ft_lstclear(&lst, del);
			ft_lstclear(&res, del);
			return (0);
		}
		ft_lstadd_back(&res, new);
		lst = lst->next;
	}
	return (res);
}
