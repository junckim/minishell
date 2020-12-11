/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:34:12 by joockim           #+#    #+#             */
/*   Updated: 2020/04/17 23:28:27 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str1, const char *str2, size_t n)
{
	size_t i;
	size_t len;

	i = 0;
	len = ft_strlen(str2);
	if (len == 0)
		return ((char *)str1);
	if (len > n)
		return (0);
	while (i < n && str1 != 0)
	{
		if (ft_strncmp((char *)&str1[i], str2, len) == 0)
		{
			if (i + len > n)
				return (0);
			return ((char *)&str1[i]);
		}
		i++;
	}
	return (0);
}
