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

/*
* *		리다이렉션이 있으면 fd 저장
**		param  : str을 포함한 노드
*/
void				get_fd(t_commands *node)
{
	t_str	*before;
	t_str	*head;

	head = node->str;
	before = node->str;
	while (node->str)
	{
		node->str = node->str->next;
	}
}

/*
* *		커맨드 노드를 만듦, 초기화
**		param  : line의 주솟값, 환경변수
**		return : 노드
*/
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

/*
* *		한 줄을 리스트의 형태로 바꿔주자.
* *		노드 (;)  노드 (;) 노드 (;) 노드 (널)
* *		(파이프) (파이프)		  (파이프)
* *		노드	  노드			   노드
* *		(널)	 (널)			  (널)
**		param  : 한 줄, 환경변수
**		return : 링크드리스트
*/
t_commands			*split_separator(char *line, t_env *env)		//	! add header
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
