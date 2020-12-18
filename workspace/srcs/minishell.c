#include "../include/minishell.h"
#include <stdio.h>

void	signal_handler(int signo)
{
	printf("\n%d\n", signo);
	printf("get signal\n");
}

void	init_check(t_check *check)
{
	check->slash = 0;
	check->quo = 0;
	check->B_quo = 0;
}

int		check_word_input(char *str, char c)
{
	int		flag;

	flag = 0;
	while (*str)
	{
		if (*str == c && flag == 0)
			flag = 1;
		else if (*str == c && flag == 1)
			flag = 0;
		str++;
	}
	return (flag);
}

void	check_input(char **input)
{
	int		aa;
	t_check	check;

	init_check(&check);
	if ((aa = check_word_input(*input, '"')))
	{
		check.B_quo = 1;
	}
	printf("%d\n", check.B_quo);
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
	check_input(input);
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

int	main(int argc, char **argv, char **envp)
{
	int			status;
	char		*input;
	t_list		*lst;
	t_list		*cur;
	int			i;		// ! erase later

	status = 1;
	while(status)
	{
		make_prompt_msg();
		get_input(&input);
		printf("input test : %s\n", input);
	/*	lst = split_separator(input);
		i = 0;
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
