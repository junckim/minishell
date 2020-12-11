/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:42:21 by joockim           #+#    #+#             */
/*   Updated: 2020/04/09 17:43:07 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	size_t	len;
	char	*res;

	len = ft_strlen(s1) + ft_strlen(s2);
	if ((res = (char *)malloc(sizeof(char) * (len + 1))) == 0)
		return (0);
	i = 0;
	while (len--)
	{
		if (*s1 != 0)
			res[i] = *s1++;
		else
			res[i] = *s2++;
		i++;
	}
	res[i] = '\0';
	return (res);
}
