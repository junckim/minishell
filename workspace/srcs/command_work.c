#include "../include/minishell.h"

int			cd_work(t_commands *node)
{
	char	*buf;

	if ((buf = getcwd(0, 200)) == NULL)
	{
		printf("can not read current dir\n");
		return (-1);
	}
	printf("current dir : %s\n", buf);
	if (chdir(node->str->next) == -1)
	{
		printf("cannot chage the path\n");
		return (-1);
	}
	if ((buf = getcwd(0, 200)) == NULL)
	{
		printf("can not read current dir\n");
		return (-1);
	}
	printf("current dir : %s\n", buf);
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
	return (1);
}

/*
**		cd #env export unset exit
*/
int			command_work(t_commands *node, t_env *env, int cmd)
{
	if (cmd == CD)
		return (cd_work(node));
	else if (cmd == ENV)
		return (env_work(env));
	// else if (cmd == EXPORT)
	// 	return (export_work(node, env));
	// else if (cmd == UNSET)
	// 	return (unset_work(node, env));
	// else if (cmd == EXIT)
	// 	return (exit_work(node, env));
	return (-1);
}