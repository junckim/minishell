/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_flags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 18:13:06 by joockim           #+#    #+#             */
/*   Updated: 2020/10/14 09:25:20 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int		ft_isdigit_pr(int c)
{
	return (c >= '0' && c <= '9');
}

void	init_flag(t_flag *t)
{
	t->dot = 0;
	t->minus = 0;
	t->width = -1;
	t->zero = 0;
	t->prec = -1;
}

int		get_int(const char *str, t_item *t)
{
	int r;

	r = 0;
	while (ft_isdigit_pr(str[t->i]))
	{
		r = r * 10 + (str[t->i] - '0');
		t->i++;
	}
	return (r);
}

void	star(t_item *t)
{
	int n;

	n = va_arg(t->arg, int);
	if (t->flag.dot == 0)
	{
		if (n < 0)
		{
			t->flag.minus = 1;
			t->flag.zero = 0;
			n *= -1;
		}
		t->flag.width = n;
	}
	if (t->flag.dot == 1)
	{
		if (n < 0)
			t->flag.dot = 0;
		t->flag.prec = n;
	}
	t->i++;
}

void	make_flag(const char *str, t_item *t)
{
	init_flag(&t->flag);
	while (str[t->i] == '-' || str[t->i] == '0')
	{
		if (str[t->i] == '-')
		{
			t->flag.minus = 1;
			t->flag.zero = 0;
		}
		else
			t->flag.zero = 1;
		t->i++;
	}
	if (ft_isdigit_pr(str[t->i]) && !t->flag.dot)
		t->flag.width = get_int(str, t);
	if (str[t->i] == '*')
		star(t);
	if (str[t->i] == '.')
	{
		t->flag.dot = 1;
		t->i++;
		if (ft_isdigit_pr(str[t->i]))
			t->flag.prec = get_int(str, t);
		if (str[t->i] == '*' && t->flag.prec == -1)
			star(t);
	}
}
