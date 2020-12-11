/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:36:18 by joockim           #+#    #+#             */
/*   Updated: 2020/04/09 17:37:46 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	size_t	i;
	char	*ret;

	i = 0;
	ret = 0;
	while (str[i] != 0)
	{
		if (str[i] == c)
			ret = (char *)&str[i];
		i++;
	}
	if (str[i] == c)
		ret = (char *)&str[i];
	return (ret);
}
