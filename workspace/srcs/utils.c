/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 15:09:51 by joockim           #+#    #+#             */
/*   Updated: 2020/12/30 14:18:57 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		skip_space(char **str)
{
	while (**str == 32 || (**str > 7 && **str < 13))
		(*str)++;
}

int         atoi(const char *fd)
{
    int     ret;

    ret = 0;
    while (*fd)
    {
        ret *= 10;
        ret += (*fd) - '0';
        fd++;
    }
    return (ret);
}

int		ft_isspace(char c)
{
	if (c == 32 || (c > 7 && c < 13))
		return (1);
	return (0);
}

int		ft_isset(char c, const char *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

void	*err_malloc(unsigned int n)  // error 시 exit 대신 에러 출력
{
	void	*ptr;

	ptr = malloc(n);
	if (ptr == NULL)
		exit(1);
	return (ptr);
}
