#include "../include/minishell.h"

int			redir_in_node(t_commands *node)
{
	t_str	*str;
	
	str = node->str;
	while (str)
	{
		if (str->redir > 0)
		{
			0;
		}
		str = str->next;
	}
}

int			work_redir(t_commands *lst)
{
	int		err_num;
	while (lst)
	{
		if ((err_num = redir_in_node(lst)) < 0)
		{
			error_check(err_num, "");
		}
		if (lst->pipe)
		{
			go_pipe(lst->pipe);
		}
		lst = lst->next;
	}
}