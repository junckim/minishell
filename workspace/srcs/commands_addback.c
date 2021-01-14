/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_addback.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junkang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:38:50 by junkang           #+#    #+#             */
/*   Updated: 2021/01/14 19:38:58 by junkang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_commands	*lstlast_next(t_commands *lst)
{
	while (lst)
	{
		if (lst->next == 0)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

static t_commands	*lstlast_pipe(t_commands *lst)
{
	while (lst)
	{
		if (lst->pipe == 0)
			return (lst);
		lst = lst->pipe;
	}
	return (lst);
}

void				commands_addback(t_commands **lst, t_commands *new)
{
	t_commands		*last_semi;
	t_commands		*last_node;
	t_commands		*tmp;

	if (new == 0 || lst == 0)
		return ;
	if (*lst == 0)
	{
		*lst = new;
		return ;
	}
	last_semi = lstlast_next(*lst);
	if (last_semi->sep == PIPE)
		last_node = lstlast_pipe(last_semi);
	else
		last_node = last_semi;
	if (last_node->sep == PIPE)
	{
		last_node->pipe = new;
	}
	else if (last_node->sep == SEMI)
		last_semi->next = new;
}
