/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_p.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 16:49:48 by joockim           #+#    #+#             */
/*   Updated: 2020/10/11 19:29:16 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

char	*ft_ptoa(unsigned long long p, t_item *t, char *base)
{
	unsigned long long	n;
	int					len;
	char				*res;
	int					a;

	len = 0;
	n = p;
	if (p == 0 && !t->flag.dot)
		return (ft_strdup_pr("0"));
	while (n)
	{
		n /= 16;
		len++;
	}
	res = (char *)malloc(sizeof(char) * (len + 1));
	res[len] = 0;
	while (len--)
	{
		a = p % 16;
		res[len] = base[a];
		p /= 16;
	}
	return (res);
}

int		write_space_p(int len)
{
	int	res;

	res = len;
	while (len--)
		write(1, " ", 1);
	return (res);
}

void	case_p(t_item *t, int *res)
{
	unsigned long long	n;
	char				*str;
	int					str_len;

	n = va_arg(t->arg, unsigned long long);
	str = ft_ptoa(n, t, "0123456789abcdef");
	str_len = ft_strlen_pr(str) + 2;
	if (t->flag.minus || t->flag.width <= str_len)
	{
		*res += write(1, "0x", 2);
		*res += write(1, str, str_len - 2);
	}
	if (t->flag.width > str_len)
		*res += write_space_p(t->flag.width - str_len);
	if (!t->flag.minus && t->flag.width > str_len)
	{
		*res += write(1, "0x", 2);
		*res += write(1, str, str_len - 2);
	}
	clear_point(str, str_len - 2);
}
