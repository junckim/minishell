/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_cs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 16:49:01 by joockim           #+#    #+#             */
/*   Updated: 2020/10/11 19:20:53 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

void	case_c(t_item *t, int *res)
{
	char	c;

	c = va_arg(t->arg, int);
	if (t->flag.minus)
		*res += putchar_len(&c, 1);
	while (--t->flag.width > 0)
		*res += write(1, " ", 1);
	if (!t->flag.minus)
		*res += putchar_len(&c, 1);
}

int		write_space_s(t_item *t, int len)
{
	int	res;

	res = len;
	while (len--)
	{
		if (t->flag.zero && !t->flag.minus)
			write(1, "0", 1);
		else
			write(1, " ", 1);
	}
	return (res);
}

void	case_s(t_item *t, int *res)
{
	char	*s;
	int		str_len;

	s = va_arg(t->arg, char*);
	if (s == 0)
		s = "(null)";
	str_len = ft_strlen_pr(s);
	if (t->flag.dot && t->flag.prec < str_len)
		str_len = t->flag.prec;
	if (t->flag.dot && t->flag.prec == -1)
		str_len = 0;
	if (t->flag.minus || t->flag.width <= str_len)
		*res += write(1, s, str_len);
	if (t->flag.width > str_len)
		*res += write_space_s(t, t->flag.width - str_len);
	if (!t->flag.minus && t->flag.width > str_len)
		*res += write(1, s, str_len);
}

void	case_per(t_item *t, int *res)
{
	if (t->flag.minus)
		*res += write(1, "%", 1);
	if (t->flag.width > 1)
		*res += write_space_s(t, t->flag.width - 1);
	if (!t->flag.minus)
		*res += write(1, "%", 1);
}
