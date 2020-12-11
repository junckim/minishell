/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:38:08 by joockim           #+#    #+#             */
/*   Updated: 2020/04/15 17:45:53 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t len;

	len = ft_strlen(src);
	if (len < n)
	{
		while (*src)
		{
			*dest = *src;
			dest++;
			src++;
		}
		*dest = 0;
	}
	else
	{
		while (n--)
		{
			*dest = *src;
			dest++;
			src++;
		}
		*(dest - 1) = 0;
	}
	return (len);
}
