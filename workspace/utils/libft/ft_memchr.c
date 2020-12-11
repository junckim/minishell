/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 18:19:47 by joockim           #+#    #+#             */
/*   Updated: 2020/04/09 18:19:53 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char *a;
	unsigned char b;

	a = (unsigned char *)s;
	b = c;
	while (n--)
	{
		if (*a == b)
			return (a);
		a++;
	}
	return (0);
}
