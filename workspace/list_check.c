#include "../include/minishell.h"


int		node_check(t_commands *lst)
{
	if (lst->str == NULL)
		return (-1);
	return (1);
}

int		pipe_check(t_commands *pipe_lst)
{
	while (pipe_lst)
	{
		if (pipe_lst->str == NULL)
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
		if ((ret = node_check(lst)) < 0)
			return (ERR_EMPTY_SEMI);
		if (lst->pipe)
		{
			if ((ret = pipe_check(lst->pipe)) < 0)
				return (ERR_EMPTY_PIPE);
		}
		lst = lst->next;
	}
	return (1);
}