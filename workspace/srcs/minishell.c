#include "../include/minishell.h"
#include <stdio.h>

void	signal_handler(int signo)
{
	printf("\n%d\n", signo);
	printf("get signal\n");
}

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
}

void	make_prompt_msg(char *path)
{
	char *last;

	path = ft_strrchr(path, '/');
	ft_printf(COLOR_MAGENTA);
	ft_printf("dir :");
	ft_printf(COLOR_BCYAN);
	ft_printf("%s", last);
	ft_printf(COLOR_BRED);
	ft_printf(" @~"COLOR_BYELLOW"=+"COLOR_BWHITE"=+"
			COLOR_BGREEN"=+>");
	ft_printf(COLOR_RESET);
}

void	add_newenv(t_env *env, char *str)
{
	t_env	*head;
	t_env	*elem;
	char	*point;
	
	point = ft_strchr(str, '=');
	elem = malloc(sizeof(t_env));
	if (env)
	{
		while (env->next)
			env = env->next;
		env->next = elem;
	}
	else
		env = elem;
	*point = '\0';
	elem->key = str;
	elem->value = point + 1;
	elem->next = NULL;
}

t_env	make_env(char **envp)
{
	t_env	*env;
	t_env	*test;
	char	*point;

	while (*envp)
	{
		env = add_newenv(env, *envp);
		envp++;
	}
	return (*env);
}

int	main(int argc, char **argv, char **envp)
{
	t_env		env;
	int			status;
	char		*input;
	char		*path;
	t_list		*lst;
	t_list		*cur;
	int			i;		// ! erase later

	status = 1;
	env = make_env(envp);
/*	path = getcwd(NULL, 0);
	while(status)
	{
		make_prompt_msg(path);
		get_input(&input);
		printf("input test : %s\n", input);
		lst = split_separator(input);
		i = 0;
		cur = lst;
		while (cur)
		{
			// skip_space(cur);
			printf("line[%d]\ncommand : %d\nstr : %s\nsep : %d\n", i++, cur->content->command, cur->content->str, cur->content->sep);
			// command_branch(*cur); // 만들어진 문자리스트 && t_argu
			cur = cur->next;
		}
	}*/
	return (0);
}
