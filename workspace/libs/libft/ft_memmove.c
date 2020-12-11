/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:46:39 by joockim           #+#    #+#             */
/*   Updated: 2020/04/12 22:57:10 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	unsigned char	*last_d;
	unsigned char	*last_s;

	if (dest == 0 && src == 0)
		return (0);
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (d < s)
	{
		while (n--)
			*d++ = *s++;
	}
	else
	{
		last_d = d + (n - 1);
		last_s = s + (n - 1);
		while (n--)
			*last_d-- = *last_s--;
	}
	return (dest);
}
