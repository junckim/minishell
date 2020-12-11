/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:29:30 by joockim           #+#    #+#             */
/*   Updated: 2020/04/09 17:32:11 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		isset(char c, char const *set)
{
	int res;

	res = 1;
	while (*set)
	{
		if (*set == c)
			res = 0;
		set++;
	}
	return (res);
}

int		get_front(char const *s1, char const *set)
{
	int	i;
	int res;

	i = 0;
	res = 0;
	while (s1[i])
	{
		if (isset(s1[i], set))
		{
			res = i;
			break ;
		}
		i++;
	}
	return (res);
}

int		get_back(char const *s1, char const *set, int len)
{
	int res;

	res = 0;
	while (len)
	{
		if (isset(s1[len], set))
		{
			res = len;
			break ;
		}
		len--;
	}
	return (res);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		front;
	int		copy_len;
	char	*res;

	front = get_front(s1, set);
	copy_len = get_back(s1, set, ft_strlen(s1) - 1) - front + 1;
	if (copy_len == 1)
	{
		res = malloc(sizeof(char));
		*res = 0;
		return (res);
	}
	if ((res = (char *)malloc(sizeof(char) * copy_len + 1)) == 0)
		return (0);
	i = 0;
	while (copy_len--)
		res[i++] = s1[front++];
	res[i] = '\0';
	return (res);
}
