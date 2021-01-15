/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   branch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 15:06:27 by joockim           #+#    #+#             */
/*   Updated: 2020/12/16 17:52:22 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_error_status;

t_commands			*make_commands_new(char **ref, t_env *env)
{
	t_commands		*node;

	node = (t_commands *)err_malloc(sizeof(t_commands));
	node->sep = -1;
	node->command = -1;
	node->str = NULL;
	node->fd = 0;
	node->fdflag = 0;
	node->pipe = NULL;
	node->prev = NULL;
	node->next = NULL;
	parse_node(ref, node, env);
	return (node);
}

t_commands			*split_separator(char *line, t_env *env)
{
	t_commands		*ret;
	t_commands		*node;

	ret = 0;
	while (*line)
	{
		node = make_commands_new(&line, env);
		commands_addback(&ret, node);
	}
	return (ret);
}
