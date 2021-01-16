/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 18:42:38 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 18:47:50 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern char	*g_read_str;
extern int	g_error_status;

int		status_return(int status)
{
	if (ft_ifsignal(status))
		return (128 + status);
	if (ft_exitstatus(status) != 255)
		return (ft_exitstatus(status));
	return (255);
}

int		path_work(t_commands *node, char *path, t_env *env)
{
	int		pid;
	char	**argv;
	char	**envp;
	int		status;
	int		res;

	if ((pid = fork()) == -1)
		return (-2);
	else if (pid == 0)
	{
		argv = str_to_argv(node);
		envp = env_to_envp(env);
		if (node->fdflag == 1)
			dup2(node->fd, STDOUT_FILENO);
		else if (node->fdflag == 2)
			dup2(node->fd, STDIN_FILENO);
		res = execve(path, argv, envp);
		exit(res);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	return (status_return(status));
}

int		excute_work(t_commands *node, t_env *env)
{
	char	*path;

	path = node->str->word;
	return (path_work(node, path, env));
}

int		path_excute(t_commands *node, t_env *env, t_path *path)
{
	int		res;
	char	*word;
	char	*tmp;

	word = ft_strdup(node->str->word);
	while (path)
	{
		tmp = triple_join(path->path, "/", word);
		free(node->str->word);
		node->str->word = tmp;
		if ((res = excute_work(node, env)) != 255)
		{
			free(word);
			word = NULL;
			return (res);
		}
		path = path->next;
	}
	free(word);
	word = NULL;
	return (127);
}

void	work_command(t_commands *node, t_env **env)
{
	t_path	*path;
	int		cmd;

	path = make_path_lst(*env);
	if (work_redir(node) == SYS_SYNTAX)
	{
		g_error_status = SYS_SYNTAX;
		free_path(&path);
		// free_all_node(&node);
		return ;
	}
	if (node->str->word[0] == '/')
	{
		if (excute_work(node, *env) == 255)
			g_error_status = 127;
	}
	else
	{
		if ((cmd = is_command(node->str->word)) == -1)
			g_error_status = path_excute(node, *env, path);
		else
			g_error_status = command_work(node, env, cmd);
	}
	if (g_error_status == 127)
		error_check(SYS_CMD_NOT_FOUND, ft_strrchr(node->str->word, '/') + 1);
	free_path(&path);
}
