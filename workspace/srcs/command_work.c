#include "../include/minishell.h"

int			cd_work(t_commands *node, t_env *env)
{
	char	*buf;

	if ((buf = getcwd(0, 200)) == NULL)
	{
		printf("can not read current dir\n");
		return (-1);
	}
	add_change_env(env, "OLDPWD", buf);
	if (node->str->next == 0)
	{
		if (chdir(get_value(env, "HOME")) == -1)
		{
			printf("cannot change the path\n");
			return (-1);
		}
	}
	else if (chdir(node->str->next->word) == -1)
	{
		printf("cannot chage the path\n");
		return (-1);
	}
	if ((buf = getcwd(0, 200)) == NULL)
	{
		printf("can not read current dir\n");
		return (-1);
	}
	add_change_env(env, "PWD", buf);
	free(buf);
	return (1);
}

int			env_work(t_env *env)
{
	char		**envp;
	int			i;

	printf("env pointer %p\n", env);
	envp = env_to_envp(env);
	i = -1;
	while (envp[++i])
		ft_printf("%s\n", envp[i]);
	return (1);
}

int			export_work(t_commands *node, t_env *env)
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

t_env	*env_prev_cur(t_env *env, char *key, t_env **prev)
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

int			unset_work(t_commands *node, t_env **env)
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
			{
				*env = (*env)->next;
				free(cur_env->key);
				free(cur_env->value);
				free(cur_env);
			}
			else
			{
				prev_env->next = cur_env->next;
				free(cur_env->key);
				free(cur_env->value);
				free(cur_env);
			}
		}
		cur = cur->next;
	}
	return (1);
}

void		exit_work(t_commands *node, t_env *env)
{
	int		num;

	num = 0;
	if (node->str->next)
		num = ft_atoi(node->str->next->word);
	exit(num);
}

/*
**		cd #env export unset exit
*/
int			command_work(t_commands *node, t_env **env, int cmd)
{
	if (cmd == CD)
		return (cd_work(node, *env));
	else if (cmd == ENV)
		return (env_work(*env));
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