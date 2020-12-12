/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   branch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 15:06:27 by joockim           #+#    #+#             */
/*   Updated: 2020/12/11 15:24:51 by kimjoocha        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_str_echo_cmp(char *command)
{
	if (ft_strncmp(command, "echo", 4) == 0)
		return (1);
	else if (ft_strncmp(command, "Echo", 4) == 0)
		return (1);
	else if (ft_strncmp(command, "\'echo\'", 6) == 0)
		return (1);
	else if (ft_strncmp(command, "\"echo\"", 6) == 0)
		return (1);
	else if (ft_strncmp(command, "\'Echo\'", 6) == 0)
		return (1);
	else if (ft_strncmp(command, "\"Echo\"", 6) == 0)
		return (1);
	return (0);
}

void	remove_quotation(char **command)
{
	return ;
}

void	command_branch(char *command)
{
	if (command[0] == '\'' || command[0] == '\"')
		remove_quotation(&command);
	if (ft_str_echo_cmp(command))
		printf("I found echo!\n");		// 동작할 함수를 넣어보자
}