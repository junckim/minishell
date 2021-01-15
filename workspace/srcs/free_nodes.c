/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 17:03:10 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 17:03:12 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	clear_str_node(t_str **str)
{
	t_str	*tmp;

	while (*str)
	{
		tmp = *str;
		*str = (*str)->next;
		free(tmp->word);
		tmp->word = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void	pipe_clear(t_commands **node)
{
	t_commands	**pipe_node;
	t_commands	*tmp;

	pipe_node = node;
	while (*pipe_node)
	{
		tmp = *pipe_node;
		clear_str_node(&(*pipe_node)->str);
		*pipe_node = (*pipe_node)->pipe;
		free(tmp);
		tmp = NULL;
	}
}

void	clear_node(t_commands **node)
{
	t_commands	*tmp;

	tmp = *node;
	clear_str_node(&(*node)->str);
	*node = (*node)->next;
	free(tmp);
	tmp = NULL;
}

void	free_all_node(t_commands **node)
{
	while (*node)
	{
		if ((*node)->pipe)
			pipe_clear(&(*node)->pipe);
		clear_node(node);
	}
}