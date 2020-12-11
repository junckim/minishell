/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/30 16:09:20 by joockim           #+#    #+#             */
/*   Updated: 2020/10/11 19:28:02 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	clear_point(char *p, int len)
{
	while (len--)
		p[len] = 0;
	free(p);
	p = 0;
}

int		ft_strlen_pr(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int		putchar_len(char *str, int len)
{
	write(1, str, len);
	return (len);
}

char	*ft_strdup_pr(const char *string)
{
	int		len;
	char	*s;
	char	*res;

	len = ft_strlen_pr((char *)string);
	if (!(s = (char *)malloc(sizeof(char) * len + 1)))
		return (0);
	res = s;
	while (*string)
		*s++ = *string++;
	*s = 0;
	return (res);
}
