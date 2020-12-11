/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:53:11 by joockim           #+#    #+#             */
/*   Updated: 2020/04/12 22:56:28 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (dest == 0 && src == 0)
		return (0);
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	while (size--)
		*d++ = *s++;
	return (dest);
}
