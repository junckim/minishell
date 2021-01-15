/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 18:10:04 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 18:10:06 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*triple_join(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

void	kill_process(int pid)
{
	if (pid == 0)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
	else
		kill(pid, SIGKILL);
}

void	check_d(int *ret, char *buf, char *str)
{
	if (ft_strlen(str))
	{
		*ret = 1;
		buf[0] = 0;
		write(1, "  \b\b", 4);
	}
	else
	{
		kill_process(0);
	}
}

int		strcmp_upper(const char *command, const char *str)
{
	int		i;
	char	c;

	i = -1;
	while (command[++i])
	{
		if ('A' <= str[i] && str[i] <= 'Z')
			c = str[i] - 'A' + 'a';
		else
			c = str[i];
		if (c != command[i])
			return (0);
	}
	return (1);
}
