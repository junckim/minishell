/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:41:09 by joockim           #+#    #+#             */
/*   Updated: 2020/10/10 23:28:20 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *string)
{
	size_t	len;
	char	*s;
	char	*ret;

	len = ft_strlen(string);
	if ((s = (char *)malloc(sizeof(char) * len + 1)) == 0)
		return (0);
	ret = s;
	while (len--)
		*s++ = *string++;
	*s = 0;
	return (ret);
}
