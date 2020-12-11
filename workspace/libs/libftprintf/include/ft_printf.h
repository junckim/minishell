/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 18:15:29 by joockim           #+#    #+#             */
/*   Updated: 2020/10/14 09:22:42 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef	struct	s_flag
{
	int	dot;
	int minus;
	int	star;
	int	width;
	int	zero;
	int prec;
}				t_flag;

typedef	struct	s_item
{
	int		i;
	va_list	arg;
	t_flag	flag;
}				t_item;

int				ft_printf(const char *str, ...);
int				handle(const char *str, t_item *t, int *res);
void			init_flag(t_flag *t);
int				get_int(const char *str, t_item *t);
void			star(t_item *t);
void			make_flag(const char *str, t_item *t);
int				ft_isdigit_pr(int c);
int				ft_strlen_pr(char *str);
int				putchar_len(char *str, int len);
char			*ft_strdup_pr(const char *string);
void			clear_point(char *p, int len);
int				get_n_len(long int n);
char			*ft_itoa_pr(long int n, t_item *t);
int				print_zero_space(t_item *t, int len);
void			case_d(t_item *t, int *res);
void			case_c(t_item *t, int *res);
int				write_space_s(t_item *t, int len);
void			case_s(t_item *t, int *res);
void			case_per(t_item *t, int *res);
char			*ft_ptoa(unsigned long long p, t_item *t, char *base);
int				write_space_p(int len);
void			case_p(t_item *t, int *res);
int				get_u_len(unsigned int n, int div);
char			*ft_utoa(unsigned int n, t_item *t);
void			case_u(t_item *t, int *res);
char			*ft_xtoa(unsigned int n, t_item *t, char *base);
void			case_x(t_item *t, int *res, char c);
#endif
