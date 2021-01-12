/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excute_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 16:08:49 by joockim           #+#    #+#             */
/*   Updated: 2021/01/13 03:20:42 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		main(int argc, char **argv, char **envp)
{
	char	*path;

	path = getcwd(0, 0);
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(path);
	return (0);
}
