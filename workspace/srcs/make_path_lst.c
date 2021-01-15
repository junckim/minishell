/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_path_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 16:42:17 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 16:59:55 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_path(t_path **path)
{
	t_path	*tmp;

	while (*path)
	{
		tmp = *path;
		*path = (*path)->next;
		free(tmp->path);
		tmp->path = NULL;
		free(tmp);
		tmp = NULL;
	}
}

t_path	*new_path_one(char *str)
{
	t_path	*res;

	res = (t_path *)err_malloc(sizeof(t_path));
	res->path = str;
	res->next = NULL;
	return (res);
}

t_path	*add_path(t_path *path, char *str)
{
	t_path	*head;

	head = path;
	if (path == NULL)
	{
		path = new_path_one(str);
		return (path);
	}
	else
	{
		while (path->next)
			path = path->next;
		path->next = new_path_one(str);
	}
	return (head);
}

t_path	*make_path_lst(t_env *env)
{
	t_path	*res;
	char	*path;
	char	*point;

	res = NULL;
	if ((path = get_value(env, "PATH")) == NULL)
		return (add_path(res, ft_strdup("")));
	point = ft_strchr(path, ':');
	while (path)
	{
		if (point)
		{
			res = add_path(res, ft_substr(path, 0, point - path));
			path = point + 1;
			point = ft_strchr(path, ':');
		}
		else
		{
			res = add_path(res, ft_strdup(path));
			path = NULL;
		}
	}
	return (res);
}
