/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 14:21:59 by joockim           #+#    #+#             */
/*   Updated: 2021/01/12 14:22:00 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		node_check(t_commands *lst)
{
	if (lst->str->redir == -1 && lst->str->word == 0)
		return (-1);
	return (1);
}

int		pipe_check(t_commands *pipe_lst)
{
	while (pipe_lst)
	{
		if (ft_strlen(pipe_lst->str->word) == 0)
			return (-1);
		pipe_lst = pipe_lst->pipe;
	}
	return (1);
}

/*
**		정상이면 1, 아니면 에러넘
*/
int		list_check(t_commands *lst)
{
	int			ret;

	while (lst)
	{
		if (lst->pipe)
		{
			if ((ret = pipe_check(lst->pipe)) < 0)
				return (ERR_EMPTY_PIPE);
		}
		if ((ret = node_check(lst)) < 0)
			return (ERR_EMPTY_SEMI);
		lst = lst->next;
	}
	return (1);
}
