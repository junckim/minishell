/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 18:23:12 by joockim           #+#    #+#             */
/*   Updated: 2020/04/13 00:35:53 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t n, size_t size)
{
	char	*item;

	item = (char *)malloc(size * n);
	if (item == 0)
		return (0);
	ft_memset(item, 0, size * n);
	return ((void *)item);
}
