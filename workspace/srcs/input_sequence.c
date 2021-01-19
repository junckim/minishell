/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 17:16:19 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 17:16:21 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	*g_read_str;
extern int	g_error_status;

int		check_input(char *str)
{
	int	flag;

	flag = 0;
	while (*str)
	{
		if (*str == '\\' && *(str + 1) != 0 && flag != SQU)
			str++;
		else if (*str == '"' && flag == 0)
			flag = BQU;
		else if (*str == '\'' && flag == 0)
			flag = SQU;
		else if (*str == '"' && flag == 1)
			flag = 0;
		else if (*str == '\'' && flag == 2)
			flag = 0;
		else if (*str == '\\' && flag == 0 && *(str + 1) == 0)
			flag = BSL;
		str++;
	}
	return (flag);
}

int		get_input(char **input)
{
	int		ret;
	char	buf[2];
	char	*temp;

	ret = 1;
	buf[0] = 0;
	buf[1] = 0;
	g_read_str = ft_strjoin("", "");
	while (ret && buf[0] != '\n')
	{
		ret = read(0, buf, 1);
		if (ret == 0)
			check_d(&ret, buf, g_read_str);
		if (buf[0] != '\n' && ret != 0)
		{
			temp = ft_strjoin(g_read_str, buf);
			free(g_read_str);
			g_read_str = temp;
		}
	}
	*input = g_read_str;
	return (check_input(g_read_str));
}

void	slash_doing(char **input)
{
	int		flag;
	char	*tmp;
	char	*more;

	write(1, ">", 1);
	tmp = ft_substr(*input, 0, ft_strlen(*input) - 1);
	free(*input);
	flag = get_input(&more);
	*input = ft_strjoin(tmp, more);
	free(more);
	free(tmp);
	if (flag == BSL)
		slash_doing(input);
}

void	quo_doing(char **input, int quo)
{
	int		flag;
	char	*temp;
	char	*more;

	if (quo == SQU)
		write(1, "quote>", 6);
	else
		write(1, "D_quote>", 8);
	temp = ft_strjoin(*input, "\n");
	free(*input);
	flag = get_input(&more);
	*input = ft_strjoin(temp, more);
	free(more);
	free(temp);
	if (flag != quo)
		quo_doing(input, quo);
}

void	input_sequence(char **input)
{
	int	flag;

	if (g_error_status != 130)
		make_prompt_msg();
	flag = get_input(input);
	if (flag == BSL)
		slash_doing(input);
	else if (flag == SQU)
		quo_doing(input, SQU);
	else if (flag == BQU)
		quo_doing(input, BQU);
}
