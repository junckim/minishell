/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 14:21:59 by joockim           #+#    #+#             */
/*   Updated: 2021/01/14 19:39:46 by junkang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		node_check(t_commands *lst)
{
	if (lst->str->redir == -1 && ft_strlen(lst->str->word) == 0)
	{
		if (lst->sep == PIPE)
			return (ERR_EMPTY_PIPE);
		else if (lst->sep == SEMI)
			return (ERR_EMPTY_SEMI);
		else
			return (ERR_EMPTY_NEWLINE);
	}
	return (1);
}

int		pipe_check(t_commands *pipe_lst)
{
	while (pipe_lst)
	{
		if (pipe_lst->str->redir == -1 && ft_strlen(pipe_lst->str->word) == 0)
		{
			if (pipe_lst->sep == PIPE)
				return (ERR_EMPTY_PIPE);
			else if (pipe_lst->sep == SEMI)
				return (ERR_EMPTY_SEMI);
			else
				return (ERR_EMPTY_NEWLINE);
		}
		pipe_lst = pipe_lst->pipe;
	}
	return (1);
}

int		list_check(t_commands *lst)
{
	int			ret;

	while (lst)
	{
		if ((ret = node_check(lst)) != 1)
			return (ret);
		if (lst->pipe)
		{
			if ((ret = pipe_check(lst->pipe)) != 1)
				return (ret);
		}
		lst = lst->next;
	}
	return (1);
}
