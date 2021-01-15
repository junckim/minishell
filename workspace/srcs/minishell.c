/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 22:11:19 by joockim           #+#    #+#             */
/*   Updated: 2021/01/10 18:10:47 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// 문자열 생성용 전역변수 -> ctrl + C에서 처리해주기 위함
char	*g_read_str;
int		g_error_status;

void	make_prompt_msg(void)
{
	char	*path;
	char	*last;

	path = getcwd(NULL, 0);
	last = ft_strrchr(path, '/');
	ft_printf(COLOR_MAGENTA);
	ft_printf("dir :");
	ft_printf(COLOR_BCYAN);
	ft_printf("%s", last);
	ft_printf(COLOR_BRED);
	ft_printf(" @~"COLOR_BYELLOW"=+"COLOR_BWHITE"=+"
			COLOR_BGREEN"=+>");
	ft_printf(COLOR_RESET);
}

void	signal_handler(int signo)
{
	char	*tmp;

	write(1, "\b\b  \b\b", 6);
	if (signo == SIGINT)
	{
		if (*g_read_str)
		{
			tmp = ft_strdup("");
			free(g_read_str);
			g_read_str = tmp;
		}
		g_error_status = 1;
		write(1, "\n", 1);
		make_prompt_msg();
	}
}

void	signal_func(void)
{
	signal(SIGINT, (void *)signal_handler);
	signal(SIGQUIT, (void *)signal_handler);
}

/*
**       
**       env 관련
**
*/
t_env	*envp_to_env(char *envp)
{
	t_env	*env;
	char	*path;
	char	*key;
	char	*value;

	path = ft_strchr(envp, '=');
	key = envp;
	value = path + 1;
	*path = 0;
	if ((env = malloc(sizeof(t_env))) == 0)
		return (0);
	env->key = ft_strdup(key);
	env->value = ft_strdup(value);
	env->next = NULL;
	return (env);
}

void	add_envlst(t_env *env, char *envp)
{
	t_env	*elem;

	elem = envp_to_env(envp);
	while (env->next)
		env = env->next;
	env->next = elem;
}


t_env	*make_envlst(char **envp)
{
	t_env	*env;

	env = envp_to_env(*envp);
	envp++;
	while (*envp)
	{
		add_envlst(env, *envp);
		envp++;
	}
	return (env);
}


char	*get_value(t_env *env, char *key)
{
	char	*value;

	value = NULL;
	while (env)
	{
		if (!ft_strncmp(key, env->key, ft_strlen(key)) &&
				!ft_strncmp(key, env->key, ft_strlen(env->key)))
			value = env->value;
		env = env->next;
	}
	return (value);
}

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

char	*triple_join(char *s1, char *s2, char *s3); // 프로토타입 나중에 옮기면서 지우기

void	add_own_path(t_env *env)
{
	char	*temp;
	char	*excute_path;
	t_env	*path_env;

	path_env = get_env_pointer(env, "PATH");
	excute_path = getcwd(0, 0); // 지금은 현재 디렉이지만 나중엔 바꿔줘
	printf("%s\n", excute_path);
	temp = triple_join(excute_path, ":", path_env->value);
	free(path_env->value);
	path_env->value = temp;
}

t_env	*set_env_lst(char **envp)
{
	int		shlvl_tmp;
	t_env	*env;
	char *test;

	env = make_envlst(envp);
	g_error_status = 0;
	shlvl_tmp = ft_atoi(get_value(env, "SHLVL"));
	add_change_env(env, "SHLVL", ft_itoa(++shlvl_tmp));
	add_own_path(env);
	return(env);
}

/*
**
**			input처리
**
*/

void	kill_process(int pid)
{
	if (pid == 0)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
	else
		kill(pid, SIGKILL);
}

void	check_d(int *ret, char *buf, char *str)
{
	if (ft_strlen(str))
	{
		*ret = 1;
		buf[0] = 0;
		write(1, "  \b\b", 4);
	}
	else
	{
		kill_process(0);
	}
}

/*
**
**		command시작부분
**
*/

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

/*
**		cd #env export unset exit
*/
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

char	*triple_join(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

char	**env_to_envp(t_env *env)
{
	char	**work;
	char	**ret;
	int		size;
	char	*str;
	char	*tmp;

	work = (char **)err_malloc(sizeof(char *) * ((size = lstsize_env(env)) + 1));
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

/*
**		echo pwd ls
*/

int		ft_exitstatus(int status)
{
	return ((status >> 8) & 0x000000ff);
}

int		ft_ifsignal(int status)
{
	return ((status & 0177) != 0177 && (status & 0177) != 0);
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
		waitpid(pid, &status, 0); // -> 여기서도 status 업뎃
	}
	return (status_return(status));
}

int		excute_work(t_commands *node, t_env *env)	// 성공인지 실패인지 반환
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
			return (res);		// 실행 됨
		}
		path = path->next;
	}
	free(word);
	word = NULL;
	return (127);				// 실행 안 됨
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
// echo a | echo b | echo c

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
