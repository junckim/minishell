/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_func_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 17:32:41 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 17:53:27 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_error_status;

t_env	*get_env_pointer(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strncmp(key, env->key, ft_strlen(key)) &&
				!ft_strncmp(key, env->key, ft_strlen(env->key)))
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	add_change_env(t_env *env, char *key, char *value)
{
	t_env	*cur_env;

	if ((cur_env = get_env_pointer(env, key)))
	{
		free(cur_env->value);
		cur_env->value = ft_strdup(value);
	}
	else
	{
		while (env->next)
			env = env->next;
		cur_env = (t_env *)err_malloc(sizeof(t_env));
		cur_env->key = ft_strdup(key);
		cur_env->value = ft_strdup(value);
		cur_env->next = NULL;
		env->next = cur_env;
	}
}

void	add_own_path(t_env *env)
{
	char	*temp;
	char	*excute_path;
	t_env	*path_env;

	path_env = get_env_pointer(env, "PATH");
	excute_path = getcwd(0, 0);
	temp = triple_join(excute_path, ":", path_env->value);
	free(path_env->value);
	path_env->value = temp;
}

t_env	*set_env_lst(char **envp)
{
	int		shlvl_tmp;
	t_env	*env;

	env = make_envlst(envp);
	g_error_status = 0;
	shlvl_tmp = ft_atoi(get_value(env, "SHLVL"));
	add_change_env(env, "SHLVL", ft_itoa(++shlvl_tmp));
	if (ft_strncmp(get_value(env, "SHELL"), "/minishell",
			ft_strlen("/minishell")))
		add_own_path(env);
	add_change_env(env, "SHELL", "/minishell");
	return (env);
}
