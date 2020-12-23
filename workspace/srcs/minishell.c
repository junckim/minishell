#include "../include/minishell.h"
#include <stdio.h>

int		check_input(char *str)
{
	int	flag;

	flag = 0;
	while (*str)
	{
		if (*str == '"' && flag == 0)
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

void	more_input(char **input);

void	get_input(char **input)
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
	more_input(input);
}

void	BSL_doing(char **input)
{
	char	*tmp;
	char	*more;

	tmp = ft_substr(*input, 0, ft_strlen(*input) - 1);
	free(*input);
	get_input(&more);
	*input = ft_strjoin(tmp, more);
	free(more);
	free(tmp);
}

void	more_input(char **input)
{
	int	flag;
	int	len;
	char	*tmp;
	char	*more;

	if ((flag = check_input(*input)))
		write(1, ">", 1);
	if (flag == BSL)
		BSL_doing(input);
}

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

int	main(int argc, char **argv, char **envp)
{
	int			status;
	char		*input;
	t_env		*env;
	t_list		*lst;
	t_list		*cur;
	int			i;		// ! erase later

	signal(SIGINT, (void *)signal_handler);
	status = 1;
	env = make_envlst(envp);
	while(status)
	{
		make_prompt_msg();
		get_input(&input);
		printf("input test : %s\n", input);
		lst = split_separator(input);
	/*	i = 0;
		cur = lst;
		while (cur)
		{
			// skip_space(cur);
			printf("line[%d]\ncommand : %d\nstr : %s\nsep : %d\n", i++, cur->content->command, cur->content->str, cur->content->sep);
			// command_branch(*cur); // 만들어진 문자리스트 && t_argu
			cur = cur->next;
		}*/
	}
	return (0);
}
