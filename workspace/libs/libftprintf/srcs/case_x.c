/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_x.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 16:50:23 by joockim           #+#    #+#             */
/*   Updated: 2020/10/11 19:29:34 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

char	*ft_xtoa(unsigned int n, t_item *t, char *base)
{
	int		len;
	char	*str;

	if (n == 0 && !t->flag.dot)
		return (ft_strdup_pr("0"));
	len = get_u_len(n, 16) > t->flag.prec ? get_u_len(n, 16) : t->flag.prec;
	str = (char *)malloc(sizeof(char) * (len + 1));
	str[len] = 0;
	while (len--)
	{
		str[len] = base[n % 16];
		n /= 16;
	}
	return (str);
}

void	case_x(t_item *t, int *res, char c)
{
	int		str_len;
	char	*str;

	if (c == 'x')
		str = ft_xtoa(va_arg(t->arg, unsigned int), t, "0123456789abcdef");
	else
		str = ft_xtoa(va_arg(t->arg, unsigned int), t, "0123456789ABCDEF");
	str_len = ft_strlen_pr(str);
	if (t->flag.minus)
		*res += putchar_len(str, str_len);
	if (t->flag.width > str_len)
		*res += print_zero_space(t, t->flag.width - str_len);
	if (!t->flag.minus)
		*res += putchar_len(str, str_len);
	clear_point(str, str_len);
}
