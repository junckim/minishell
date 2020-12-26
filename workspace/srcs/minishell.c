#include "../include/minishell.h"

void	make_prompt_msg()
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
	signo = 0;
	write(1, "\n", 1);
	make_prompt_msg();
}

t_env	*make_env_new(char *envp)
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
	env->key = key;
	env->value = value;
	env->next = NULL;
	return (env);
}

void	add_envlst(t_env *env, char *envp)
{
	t_env	*elem;

	elem = make_env_new(envp);
	while (env->next)
		env = env->next;
	env->next = elem;
}

t_env	*make_envlst(char **envp)
{
	t_env	*env;

	env = make_env_new(*envp);
	envp++;
	while (*envp)
	{
		add_envlst(env, *envp);
		envp++;
	}
	return (env);
}

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

int		get_input(char **input)
{
	int		ret;
	char	buf[2];
	char	*str;
	char	*temp;

	buf[1] = 0;
	str = ft_strjoin("", "");
	while ((ret = read(0, buf, 1)) && buf[0] != '\n')
	{
		if (buf[0] != '\n' && ret != 0)
		{
			temp = ft_strjoin(str, buf);
			free(str);
			str = temp;
		}
	}
	*input = str;
	return (check_input(str));
}

void	BSL_doing(char **input)
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
		BSL_doing(input);
}

void	quo_doing(char **input, int quo)
{
	int		flag;
	char	*temp;
	char	*more;
	
	write(1, ">", 1);
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
		BSL_doing(input);
	else if (flag == SQU)
		quo_doing(input, SQU);
	else if (flag == BQU)
		quo_doing(input, BQU);
}

int	main(int argc, char **argv, char **envp)
{
	int			status;
	char		*input;
	t_env		*env;
	t_list		*lst;
	t_list		*cur;

	argc = 0; argv = 0;
	signal(SIGINT, (void *)signal_handler);
	status = 1;
	env = make_envlst(envp);
	while(status)
	{
		make_prompt_msg();
		input_sequence(&input);
		printf("input test : %s\n", input);
		lst = split_separator(input, env);
		cur = lst;
		while (cur)
		{
			printf("\n==inputs==\ncommand : %d\nstr : %s\nsep : %d\nfd : %d\nredir : %d\n", ((t_inputs *)(cur->content))->command, ((t_inputs *)(cur->content))->str, ((t_inputs *)(cur->content))->sep, ((t_inputs *)(cur->content))->pr->fd, ((t_inputs *)(cur->content))->pr->redir);
			//		branch_command((t_inputs *)(cur->content));		// cmd 별로 각 명령을 수행해 줌
			cur = cur->next;
		}
	}
	return (0);
}
