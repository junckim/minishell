#include "../include/minishell.h"

void		error_check(int err_num)
{
	if (err_num == ERR_EMPTY_SEMI)
		ft_printf("minishell: %s\n", ERR_EMPTY_SEMI_S);
	else if (err_num == ERR_EMPTY_PIPE)
		ft_printf("minishell: %s\n", ERR_EMPTY_PIPE_S);
}