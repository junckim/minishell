/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/30 16:16:23 by joockim           #+#    #+#             */
/*   Updated: 2020/10/11 19:30:10 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int		get_n_len(long int n)
{
	int	i;

	i = 0;
	if (n < 0)
		n *= -1;
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa_pr(long int n, t_item *t)
{
	int		len;
	char	*str;

	if (n == 0 && !t->flag.dot)
		return (ft_strdup_pr("0"));
	len = get_n_len(n) > t->flag.prec ? get_n_len(n) : t->flag.prec;
	str = (char *)malloc(sizeof(char) * (len + 1));
	str[len] = 0;
	while (len--)
	{
		if (n < 0)
			n *= -1;
		str[len] = n % 10 + '0';
		n /= 10;
	}
	return (str);
}

int		print_zero_space(t_item *t, int len)
{
	int res;

	res = len;
	while (len--)
	{
		if (t->flag.zero && t->flag.prec <= -1 && t->flag.dot == 0)
			write(1, "0", 1);
		else
			write(1, " ", 1);
	}
	return (res);
}

void	case_d(t_item *t, int *res)
{
	long int	n;
	int			str_len;
	char		*str;

	n = va_arg(t->arg, int);
	str = ft_itoa_pr(n, t);
	str_len = ft_strlen_pr(str);
	if (n < 0)
	{
		t->flag.width--;
		if (t->flag.minus || (t->flag.zero && !t->flag.dot)
				|| t->flag.width <= str_len)
			*res += write(1, "-", 1);
	}
	if (t->flag.minus)
		*res += putchar_len(str, str_len);
	if (t->flag.width > str_len)
		*res += print_zero_space(t, t->flag.width - str_len);
	if (n < 0 && !t->flag.minus && (!t->flag.zero || t->flag.dot)
			&& t->flag.width > str_len)
		*res += write(1, "-", 1);
	if (!t->flag.minus)
		*res += putchar_len(str, str_len);
	clear_point(str, str_len);
}
