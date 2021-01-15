/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 22:11:19 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 18:20:49 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*g_read_str;
int		g_error_status;

int		strcmp_upper(const char *command, const char *str)
{
	int		i;
	char	c;

	i = -1;
	while (command[++i])
	{
		if ('A' <= str[i] && str[i] <= 'Z')
			c = str[i] - 'A' + 'a';
		else
			c = str[i];
		if (c != command[i])
			return (0);
	}
	return (1);
}

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
	char	*test;

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
	char	*tmp;

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
		return ;
	}
	if (node->str->word[0] == '/')		// 절대
	{
		g_error_status = excute_work(node, *env);	// 성공인지 실패인지 반환
	}
	else								// 상대
	{
		if ((cmd = is_command(node->str->word)) == -1)	// env, pwd, echo
			g_error_status = path_excute(node, *env, path);
		else
		{
			g_error_status = command_work(node, env, cmd);	// export, unset, exit, cd
		}
	}
	if (g_error_status == 127)
		error_check(SYS_CMD_NOT_FOUND, ft_strrchr(node->str->word, '/') + 1);
}

void	pipe_doing(t_commands *node, t_env **env)
{
	int		p1[2];
	pid_t	pid;
	int		status;

	while (node)
	{
		if (node->pipe)
		{
			pipe(p1);
			node->pipe->fd = p1[0];
		}
		if (!(pid = fork()))
		{
			if (node->pipe)
			{
				close(p1[0]);
				dup2(p1[1], STDOUT_FILENO);
			}
			if (node->fd != STDIN_FILENO && dup2(node->fd, STDIN_FILENO) < 0)
				exit(0);
			work_command(node, env);
			exit(0);
		}
		else
		{
			if (node->pipe)
				close(p1[1]);
			if (node->fd != STDIN_FILENO)
				close(node->fd);
			waitpid(pid, &status, 0); // -> status 업데이트 할 수 있도록
		}
		node = node->pipe;
	}
}

void	start_work(t_commands *node, t_env **env)
{
	while (node)
	{
		if (node->pipe)
		{
			pipe_doing(node, env);
		}
		else
		{
			work_command(node, env);
		}
		node = node->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	int				status;
	char			*input;
	t_env			*env;
	t_commands		*node;
	int				err_num;

	argc = 0;
	status = 1;
	env = set_env_lst(envp);
	signal_func();
	while(status)
	{
		make_prompt_msg();
		input_sequence(&input);
		printf("input test : %s\n", input);
		node = split_separator(input, env);
		if ((err_num = list_check(node)) < 0)
		{
			error_check(err_num, "");
			g_error_status = SYS_SYNTAX;
		}
		else
			start_work(node, &env);
		free_all_node(&node);
	}
	return (0);
}
