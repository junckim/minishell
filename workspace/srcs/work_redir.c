/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 14:21:28 by joockim           #+#    #+#             */
/*   Updated: 2021/01/14 19:38:21 by junkang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			open_fd_node(t_str *cur, t_commands *node)
{
	int	fd;

	fd = -1;
	if (cur->redir == REDIR)
	{
		node->fdflag = 1;
		fd = open(cur->next->word, O_WRONLY | O_TRUNC | O_CREAT, 0644 | O_EXCL);
	}
	else if (cur->redir == REV_REDIR)
	{
		node->fdflag = 2;
		fd = open(cur->next->word, O_RDONLY);
	}
	else if (cur->redir == D_REDIR)
	{
		node->fdflag = 1;
		fd = open(cur->next->word, O_WRONLY | O_APPEND\
								| O_CREAT, 0644 | O_EXCL);
	}
	return (fd);
}

int			change_to_errcode(int define_num)
{
	if (define_num == PIPE)
		return (ERR_EMPTY_PIPE);
	else if (define_num == SEMI)
		return (ERR_EMPTY_SEMI);
	else if (define_num == REDIR)
		return (ERR_EMPTY_REDIR);
	else if (define_num == D_REDIR)
		return (ERR_EMPTY_D_REDIR);
	else if (define_num == REV_REDIR)
		return (ERR_EMPTY_REV_REDIR);
	return (ERR_EMPTY_NEWLINE);
}

t_str		*del_str_node(t_str *prev, t_str *cur_node, t_str **head)
{
	if (prev == NULL)
		*head = cur_node->next->next;
	else
		prev->next = cur_node->next->next;
	free(cur_node->next->word);
	free(cur_node->next);
	free(cur_node->word);
	free(cur_node);
	return (prev);
}

int			check_redir(t_commands *node, t_str *prev, t_str *head)
{
	if (node->str->next == NULL)
	{
		error_check(change_to_errcode(node->sep), "");
		return (SYS_SYNTAX);
	}
	else if (node->str->next->redir != -1)
	{
		error_check(change_to_errcode(node->str->next->redir), "");
		return (SYS_SYNTAX);
	}
	else
	{
		node->fd = open_fd_node(node->str, node);
		node->str = del_str_node(prev, node->str, &head);
	}
	return (0);
}

int			work_redir(t_commands *node)
{
	t_str	*head;
	t_str	*prev;

	head = node->str;
	prev = NULL;
	while (node->str)
	{
		if (node->str->redir != -1)
		{
			if (check_redir(node, prev, head) == SYS_SYNTAX)
				return (SYS_SYNTAX);
		}
		prev = node->str;
		if (node->str != NULL)
			node->str = node->str->next;
		else
			node->str = head;
	}
	node->str = head;
	if (node->str == 0)
		make_strsadd(node, "", -1);
	return (0);
}
