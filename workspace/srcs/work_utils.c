/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 18:36:00 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 18:48:02 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		is_command(char *cmd)
{
	if (ft_strlen(cmd) == 2 && ft_strncmp("cd", cmd, 2) == 0)
		return (CD);
	else if (ft_strlen(cmd) == 6 && ft_strncmp("export", cmd, 6) == 0)
		return (EXPORT);
	else if (ft_strlen(cmd) == 5 && ft_strncmp("unset", cmd, 5) == 0)
		return (UNSET);
	else if (ft_strlen(cmd) == 4 && ft_strncmp("exit", cmd, 4) == 0)
		return (EXIT);
	return (-1);
}

int		lstsize_str(t_str *lst)
{
	int		i;
	t_str	*move;

	i = 0;
	move = lst;
	while (move != 0)
	{
		move = move->next;
		i++;
	}
	return (i);
}

char	**str_to_argv(t_commands *node)
{
	t_str	*lst;
	char	**ret;
	int		size;
	int		i;

	lst = node->str->next;
	size = lstsize_str(lst);
	ret = (char **)err_malloc(sizeof(char *) * (size + 2));
	i = 0;
	ret[i] = ft_strdup(ft_strrchr(node->str->word, '/') + 1);
	while (++i != size + 1)
	{
		ret[i] = lst->word;
		lst = lst->next;
	}
	ret[size + 1] = NULL;
	return (ret);
}

int		lstsize_env(t_env *lst)
{
	int		i;
	t_env	*move;

	i = 0;
	move = lst;
	while (move != 0)
	{
		move = move->next;
		i++;
	}
	return (i);
}

char	**env_to_envp(t_env *env)
{
	char	**work;
	char	**ret;
	int		size;
	char	*str;

	work = (char **)err_malloc(sizeof(char *) *
			((size = lstsize_env(env)) + 1));
	ret = work;
	while (env)
	{
		str = triple_join(env->key, "=", env->value);
		*work = str;
		work++;
		env = env->next;
	}
	*work = NULL;
	return (ret);
}
