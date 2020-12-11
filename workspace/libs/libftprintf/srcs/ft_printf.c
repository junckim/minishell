/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 17:09:52 by joockim           #+#    #+#             */
/*   Updated: 2020/08/19 20:28:27 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int		handle(const char *str, t_item *t, int *res)
{
	t->i += 1;
	make_flag(str, t);
	if (str[t->i] == 'd' || str[t->i] == 'i')
		case_d(t, res);
	else if (str[t->i] == 'c')
		case_c(t, res);
	else if (str[t->i] == 's')
		case_s(t, res);
	else if (str[t->i] == 'p')
		case_p(t, res);
	else if (str[t->i] == 'u')
		case_u(t, res);
	else if (str[t->i] == 'x' || str[t->i] == 'X')
		case_x(t, res, str[t->i]);
	else if (str[t->i] == '%')
		case_per(t, res);
	else
		return (-1);
	return (1);
}

int		ft_printf(const char *str, ...)
{
	int		res;
	t_item	t;

	res = 0;
	t.i = 0;
	va_start(t.arg, str);
	while (str[t.i])
	{
		if (str[t.i] == '%')
		{
			if (handle(str, &t, &res) == -1)
				return (-1);
		}
		else
		{
			write(1, &str[t.i], 1);
			res++;
		}
		t.i++;
	}
	va_end(t.arg);
	return (res);
}
