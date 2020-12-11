/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 17:59:01 by joockim           #+#    #+#             */
/*   Updated: 2020/07/16 18:16:48 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		i_len(int n)
{
	int res;

	res = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n = n / 10;
		res++;
	}
	return (res);
}

void	fill_res(char *res, int len, int n)
{
	int a;

	a = n % 10;
	if (len != 0)
		fill_res(res - 1, len - 1, n / 10);
	if (n > 0)
		*res = a + '0';
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;

	len = i_len(n);
	if (n < 0)
		len += 1;
	if (!(res = (char *)ft_calloc(len + 1, sizeof(char))))
		return (0);
	if (n == -2147483648)
	{
		*(res + len - 1) = '8';
		len -= 1;
		n /= 10;
	}
	if (n < 0)
	{
		*res = '-';
		n *= -1;
	}
	fill_res(res + len - 1, len, n);
	if (n == 0)
		*res = '0';
	return (res);
}
