/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_work.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 18:37:27 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 18:37:28 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_env	*env_prev_cur(t_env *env, char *key, t_env **prev)
{
	*prev = NULL;
	while (env)
	{
		if (!ft_strncmp(key, env->key, ft_strlen(key)) &&
				!ft_strncmp(key, env->key, ft_strlen(env->key)))
			return (env);
		*prev = env;
		env = env->next;
	}
	return (NULL);
}

static void		free_env_node(t_env *node)
{
	free(node->key);
	free(node->value);
	free(node);
}

int				unset_work(t_commands *node, t_env **env)
{
	t_str	*cur;
	char	*param;
	t_env	*cur_env;
	t_env	*prev_env;

	cur = node->str->next;
	while (cur)
	{
		param = cur->word;
		cur_env = env_prev_cur(*env, param, &prev_env);
		if (cur_env)
		{
			if (prev_env == NULL)
				*env = (*env)->next;
			else
				prev_env->next = cur_env->next;
			free_env_node(cur_env);
		}
		cur = cur->next;
	}
	return (1);
}
