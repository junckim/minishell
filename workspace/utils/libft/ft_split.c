/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:27:01 by joockim           #+#    #+#             */
/*   Updated: 2020/04/17 23:28:45 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		get_arr(char *s, char c)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (*s)
	{
		if (*s != c)
			j++;
		if (*s == c && j != 0)
		{
			j = 0;
			i++;
		}
		s++;
	}
	if (*(s - 1) != c)
		i += 1;
	return (i);
}

char	**arr_free(char **arr, int a)
{
	while (a-- > 0)
		free(arr[a]);
	free(arr);
	return (0);
}

int		get_arg(char const *s, char c, int i)
{
	int re;

	re = 0;
	while (s[i] != c && s[i] != 0)
	{
		re++;
		i++;
	}
	return (re);
}

char	**fill(char **arr, int arr_len, char const *s, char c)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	while (s[i] != '\0' && j < arr_len)
	{
		k = 0;
		while (s[i] == c)
			i++;
		arr[j] = (char *)malloc(sizeof(char) * get_arg(s, c, i) + 1);
		if (arr[j] == 0)
			return (arr_free(arr, j));
		while (s[i] != c && s[i] != '\0')
			arr[j][k++] = s[i++];
		arr[j][k] = '\0';
		j++;
	}
	arr[j] = 0;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	int		arr_len;
	char	**arr;

	if (s == 0)
		return (0);
	arr_len = get_arr((char *)s, c);
	if ((arr = (char **)malloc(sizeof(char *) * (arr_len + 1))) == 0)
		return (0);
	return (fill(arr, arr_len, s, c));
}
