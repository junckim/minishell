/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 22:11:19 by joockim           #+#    #+#             */
/*   Updated: 2020/12/31 18:31:15 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// 문자열 생성용 전역변수 -> ctrl + C에서 처리해주기 위함
char	*g_read_str;

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

	signo = 0;
	if (*g_read_str)
	{
		tmp = ft_strdup("");
		free(g_read_str);
		g_read_str = tmp;
	}
	write(1, "\n", 1);
	make_prompt_msg();
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

t_env	*set_env_lst(char **envp)
{
	int		shlvl_tmp;
	t_env	*env;
	char *test;

	env = make_envlst(envp);
	add_change_env(env, "?", "0");
	shlvl_tmp = ft_atoi(get_value(env, "SHLVL"));
	add_change_env(env, "SHLVL", ft_itoa(++shlvl_tmp));
	return(env);
}

/*
**
**			input처리
**
*/
int		check_input(char *str)
{
	int	flag;

	flag = 0;
	while (*str)
	{
		if (*str == '\\' && *(str + 1) != 0)
			str++;
		else if (*str == '"' && flag == 0)
			flag = BQU;
		else if (*str == '\'' && flag == 0)
			flag = SQU;
		else if (*str == '"' && flag == 1)
			flag = 0;
		else if (*str == '\'' && flag == 2)
			flag = 0;
		else if (*str == '\\' && flag == 0 && *(str + 1) == 0)
			flag = BSL;
		str++;
	}
	return (flag);
}

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

void	check_d(int	*ret, char *buf, char *str)
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

int		get_input(char **input)
{
	int		ret;
	char	buf[2];
	char	*str;
	char	*temp;

	ret = 1;
	buf[0] = 0;
	buf[1] = 0;
	g_read_str = ft_strjoin("", "");
	while (ret && buf[0] != '\n')
	{
		ret = read(0, buf, 1);
		if (ret == 0)
			check_d(&ret, buf, g_read_str);
		if (buf[0] != '\n' && ret != 0)
		{
			temp = ft_strjoin(g_read_str, buf);
			free(g_read_str);
			g_read_str = temp;
		}
	}
	*input = g_read_str;
	return (check_input(g_read_str));
}

void	slash_doing(char **input)
{
	int		flag;
	char	*tmp;
	char	*more;

	write(1, ">", 1);
	tmp = ft_substr(*input, 0, ft_strlen(*input) - 1);
	free(*input);
	flag = get_input(&more);
	*input = ft_strjoin(tmp, more);
	free(more);
	free(tmp);
	if (flag == BSL)
		slash_doing(input);
}

void	quo_doing(char **input, int quo)
{
	int		flag;
	char	*temp;
	char	*more;

	if (quo == SQU)
		write(1, "quote>", 6);
	else
		write(1, "D_quote>", 8);
	temp = ft_strjoin(*input, "\n");
	free(*input);
	flag = get_input(&more);
	*input = ft_strjoin(temp, more);
	free(more);
	free(temp);
	if (flag != quo)
		quo_doing(input, quo);
}

void	input_sequence(char **input)
{
	int	flag;

	flag = get_input(input);
	if (flag == BSL)
		slash_doing(input);
	else if (flag == SQU)
		quo_doing(input, SQU);
	else if (flag == BQU)
		quo_doing(input, BQU);
}

// void	pipe_doing(t_commands *node, t_env *env)
// {
// 	pid_t	pid;

// 	if (pipe(node->fd) == -1)
// 		error;
// 	if ((pid = fork()) == -1)
// 		error;
// 	else if (pid == 0)
// 	{
// 		if (node->pipe)
// 			pipe_doing(node->pipe, env);
// 		work();
// 		child;
// 	}
// 	else
// 	{
// 		wait();
// 	}
// }
// echo aaa | bbb ; nnn
// execve -> ls -> ls 앞에다가 환경변수 붙여서 돌리기
//fork() -> execve() -> 실패시 -1; -1이면 while문 돌리면서 모든 경로값 가지고 ls 붙여서 실행
void	start_work(t_commands *node, t_env *env)
{
	while (node)
	{
		if (node->pipe);
			// pipe_doing(node, env);
		else
		{
			// work();
		}
		
		node = node->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	int				status;
	char			*input;
	t_env			*env;
	t_commands		*lst;
	t_commands		*node;

	argc = 0; argv = 0;
	signal(SIGINT, (void *)signal_handler);
	status = 1;
	env = set_env_lst(envp);
	while(status)
	{
		make_prompt_msg();
		input_sequence(&input);
		printf("input test : %s\n", input);
		lst = split_separator(input, env);
		node = lst;
		start_work(node, env);
	}
	return (0);
}