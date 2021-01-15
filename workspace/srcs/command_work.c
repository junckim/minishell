/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_work.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 14:21:47 by joockim           #+#    #+#             */
/*   Updated: 2021/01/14 19:37:50 by junkang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	cd_work(t_commands *node, t_env *env)
{
	char	*buf;

	if ((buf = getcwd(0, 0)) == NULL)
		return (-1);
	add_change_env(env, "OLDPWD", buf);
	if (node->str->next == 0)
	{
		if (chdir(get_value(env, "HOME")) == -1)
			return (-1);
	}
	else if (chdir(node->str->next->word) == -1)
		return (-1);
	if ((buf = getcwd(0, 0)) == NULL)
		return (-1);
	add_change_env(env, "PWD", buf);
	free(buf);
	return (1);
}

static int	export_work(t_commands *node, t_env *env)
{
	t_str	*cur;
	char	*param;
	char	*tmp;

	cur = node->str->next;
	while (cur)
	{
		param = cur->word;
		if ((tmp = ft_strchr(param, '=')))
		{
			if (tmp == param)
			{
				error_check(ERR_EXPORT, param + 1);
				return (-1);
			}
			*tmp = 0;
			add_change_env(env, param, tmp + 1);
			*tmp = '=';
		}
		cur = cur->next;
	}
	return (1);
}

static void	exit_work(t_commands *node, t_env *env)
{
	int		num;

	num = 0;
	if (node->str->next)
		num = ft_atoi(node->str->next->word);
	exit(num);
}

int			command_work(t_commands *node, t_env **env, int cmd)
{
	if (cmd == CD)
		return (cd_work(node, *env));
	else if (cmd == EXPORT)
		return (export_work(node, *env));
	else if (cmd == UNSET)
	{
		unset_work(node, env);
		return (1);
	}
	else if (cmd == EXIT)
		exit_work(node, *env);
	return (-1);
}
