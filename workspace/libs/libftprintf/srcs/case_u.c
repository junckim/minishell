/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_u.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 16:50:01 by joockim           #+#    #+#             */
/*   Updated: 2020/10/11 19:30:27 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int		get_u_len(unsigned int n, int div)
{
	int	i;

	i = 0;
	while (n)
	{
		n /= div;
		i++;
	}
	return (i);
}

char	*ft_utoa(unsigned int n, t_item *t)
{
	int		len;
	char	*str;

	if (n == 0 && !t->flag.dot)
		return (ft_strdup_pr("0"));
	len = get_u_len(n, 10) > t->flag.prec ? get_u_len(n, 10) : t->flag.prec;
	str = (char *)malloc(sizeof(char) * (len + 1));
	str[len] = 0;
	while (len--)
	{
		str[len] = n % 10 + '0';
		n /= 10;
	}
	return (str);
}

void	case_u(t_item *t, int *res)
{
	char	*str;
	int		str_len;

	str = ft_utoa(va_arg(t->arg, unsigned int), t);
	str_len = ft_strlen_pr(str);
	if (t->flag.minus)
		*res += putchar_len(str, str_len);
	if (t->flag.width > str_len)
		*res += print_zero_space(t, t->flag.width - str_len);
	if (!t->flag.minus)
		*res += putchar_len(str, str_len);
}
