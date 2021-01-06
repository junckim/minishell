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
	if (chdir(node->str->next->word) == -1)
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
		if ((tmp = ft_strrchr(param, '=')))
		{
			*tmp = 0;
			add_change_env(env, param, tmp + 1);
			*tmp = '=';
		}
		cur = cur->next;
	}
	return (1);
}

int			unset_work(t_commands *node, t_env *env)
{
	t_str	*cur;
	char	*param;

	cur = node->str->next;
	while (cur)
	{
		param = cur->word;
		printf("param :%s\n", param);
		del_env(&env, param);
		cur = cur->next;
	}
	return (1);
}

/*
**		cd #env export unset exit
*/
int			command_work(t_commands *node, t_env *env, int cmd)
{
	if (cmd == CD)
		return (cd_work(node, env));
	else if (cmd == ENV)
		return (env_work(env));
	else if (cmd == EXPORT)
		return (export_work(node, env));
	else if (cmd == UNSET)
		return (unset_work(node, env));
	// else if (cmd == EXIT)
	// 	return (exit_work(node, env));
	return (-1);
}