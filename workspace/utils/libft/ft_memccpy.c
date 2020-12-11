/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 18:14:10 by joockim           #+#    #+#             */
/*   Updated: 2020/04/09 18:15:25 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	unsigned char b;
	unsigned char *d;
	unsigned char *s;

	b = c;
	d = dest;
	s = (unsigned char *)src;
	while (n--)
	{
		if (*s == b)
		{
			*d = *s;
			return (d + 1);
		}
		*d++ = *s++;
	}
	return (0);
}
