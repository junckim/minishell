/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 18:03:35 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 18:03:36 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	*g_read_str;
extern int	g_error_status;

void	make_prompt_msg(void)
{
	char	*path;
	char	*last;

	path = getcwd(NULL, 0);
	last = ft_strrchr(path, '/');
	ft_printf(COLOR_MAGENTA);
	ft_printf("dir :");
	ft_printf(COLOR_BCYAN);
	ft_printf("%s", last);
	ft_printf(COLOR_BRED);
	ft_printf(" @~"COLOR_BYELLOW"=+"COLOR_BWHITE"=+"
			COLOR_BGREEN"=+>");
	ft_printf(COLOR_RESET);
}

void	signal_handler(int signo)
{
	char	*tmp;

	write(1, "\b\b  \b\b", 6);
	if (signo == SIGINT)
	{
		printf("%d\n", g_error_status);
		if (*g_read_str)
		{
			tmp = ft_strdup("");
			free(g_read_str);
			g_read_str = tmp;
		}
		g_error_status = 1;
		write(1, "\n", 1);
		make_prompt_msg();
	}
}

void	signal_func(void)
{
	signal(SIGINT, (void *)signal_handler);
	signal(SIGQUIT, (void *)signal_handler);
}

int		ft_exitstatus(int status)
{
	return ((status >> 8) & 0x000000ff);
}

int		ft_ifsignal(int status)
{
	return ((status & 0177) != 0177 && (status & 0177) != 0);
}
