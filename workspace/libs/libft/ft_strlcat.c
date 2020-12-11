/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:39:02 by joockim           #+#    #+#             */
/*   Updated: 2020/04/12 21:51:43 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t	size;
	int		cat_l;

	cat_l = n - ft_strlen(dest) - 1;
	size = ft_strlen(src) + ft_strlen(dest);
	if (ft_strlen(dest) > n)
		size = ft_strlen(src) + n;
	if (cat_l > 0)
	{
		while (*dest)
			dest++;
		while (cat_l--)
		{
			if (*src == 0)
				break ;
			*dest++ = *src++;
		}
		*dest = 0;
	}
	return (size);
}
