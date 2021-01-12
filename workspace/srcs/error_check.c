/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 14:21:22 by joockim           #+#    #+#             */
/*   Updated: 2021/01/12 14:21:23 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		error_check(int err_num, char *error_message)
{
	if (err_num == ERR_EMPTY_SEMI)
		ft_printf("minishell: %s\n", ERR_EMPTY_SEMI_S);
	else if (err_num == ERR_EMPTY_PIPE)
		ft_printf("minishell: %s\n", ERR_EMPTY_PIPE_S);
	else if (err_num == ERR_EMPTY_REDIR)
		ft_printf("minishell: %s\n", ERR_EMPTY_REDIR_S);
	else if (err_num == ERR_EMPTY_REV_REDIR)
		ft_printf("minishell: %s\n", ERR_EMPTY_REV_REDIR_S);
	else if (err_num == ERR_EMPTY_D_REDIR)
		ft_printf("minishell: %s\n", ERR_EMPTY_D_REDIR_S);
	else if (err_num == ERR_EMPTY_NEWLINE)
		ft_printf("minishell: %s\n", ERR_EMPTY_NEWLINE_S);
	else if (err_num == ERR_EXPORT)
		ft_printf("minishell: export: `=%s': not a valid identifier\n", error_message);
}
