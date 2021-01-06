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

void	del_env(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*prev;

	if (*env == 0 || key == 0)
		return ;
	cur = *env;
	if (cur->key == key)
	{
		*env = cur->next;
		return ;
	}
	while (cur)
	{
		if (cur->key == key)
		{
			free(cur->key);
			free(cur->value);
			prev->next = cur->next;
			free(cur);
		}
		prev = cur;
		cur = cur->next;
	}
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

/*
**
**		path 받아오는 부분
**
*/

typedef struct	s_path
{
	char			*path;
	struct s_path	*next;
}				t_path;

t_path	*new_path_one(char *str)
{
	t_path	*res;

	res = err_malloc(sizeof(t_path));
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
	path = get_value(env, "PATH");
	point = ft_strchr(path, ':');
	while(path)
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
	else if (ft_strlen(cmd) == 3 && strcmp_upper("env", cmd))
		return (ENV);
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

	lst = node->str->next;
	size = lstsize_str(lst);
	ret = (char **)err_malloc(sizeof(char *) * (size + 2));
	i = 0;
	ret[i] = ft_strdup("ls");
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

	work = (char **)err_malloc(sizeof(char *) * ((size = lstsize_env(env))));
	ret = work;
	while (env)
	{
		if (!ft_strncmp(env->key, "?", 1))
		{
			env = env->next;
		}
		else
		{
			str = triple_join(env->key, "=", env->value);
			*work = str;
			work++;
			env = env->next;
		}
	}
	return (ret);
}

/*
**		echo pwd ls
*/
int		path_work(t_commands *node, char *path, t_env *env)
{
	int		pid;
	char	**argv;
	char	**envp;
	int		status;

	if ((pid = fork()) == -1)
		return (-2);
	else if (pid == 0)
	{
		argv = str_to_argv(node);
		envp = env_to_envp(env);
		execve(path, argv, envp);
		exit(1);
	}
	else
	{
		wait(&status);
		if (status != 0)
			return (-1);
	}
	return (1);
}


// int		is_path(char *path)
// {
// 	char			*branch;
// 	DIR				*dir;
// 	struct dirent	*rd;

// 	branch = ft_strrchr(path, '/');
// 	branch = 0;
// 	dir = opendir(path);
// 	while ((rd = readdir(dir)) != NULL)
// 	{
// 		if (ft_strlen(branch + 1) == ft_strlen(rd->d_name) && \
// 			ft_strncmp(rd->d_name, branch + 1, ft_strlen(rd->d_name)) == 0)
// 			return (1);
// 	}
// 	closedir(dir);
// 	return (0);
// }

int		excute_work(t_commands *node, t_env *env)	// 성공인지 실패인지 반환
{
	int		cmd;
	char	*path;

	path = node->str->word;
	return (path_work(node, path, env));
}

void	work_command(t_commands *node, t_env *env)
{
	t_path	*path;
	int		cmd;

	printf("%s\n", node->str->word);
	path = make_path_lst(env);
	if (node->str->word[0] == '/')		// 절대
	{
		printf("ret :%d\n", excute_work(node, env));	// 성공인지 실패인지 반환
	}
	else								// 상대
	{
		if ((cmd = is_command(node->str->word)) == -1)
		{
			// 경로만들어서 붙어주기
			// node->str->word = 경로 + 명령어;
			// excute_work(node, env);
		}
		else
		{
			command_work(node, env, cmd);//			반환이 필요없나?
		}
	}
}

void	start_work(t_commands *node, t_env *env)
{
	while (node)
	{
		if (node->pipe);
			// pipe_doing(node, env);
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
	t_commands		*lst;
	t_commands		*node;

	argc = 0;
	signal(SIGINT, (void *)signal_handler);
	status = 1;
	env = set_env_lst(envp);
	printf("test : %s\n", argv[0]);
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