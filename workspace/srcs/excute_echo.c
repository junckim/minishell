/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 18:46:50 by joockim           #+#    #+#             */
/*   Updated: 2021/01/12 18:46:52 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		main(int argc, char *argv[], char *envp[])
{
	int			i;
	int			option;

	option = 0;
	i = 1;
	if (argc != 1)
	{
		while ((ft_strlen(argv[i]) == 2) && (!(ft_strncmp(argv[i], "-n", 2))))
		{
			option = 1;
			i++;
		}
		while (i < argc)
		{
			ft_printf("%s", argv[i]);
			if (i != argc - 1)
				ft_printf(" ");
			i++;
		}
	}
	if (!option)
		ft_printf("\n");
	return (0);
}
