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

void	make_prompt_msg(t_argu items)
{
	char *path;

	path = ft_strrchr(items.cur_path, '/');
	ft_printf(COLOR_MAGENTA);
	ft_printf("dir :");
	ft_printf(COLOR_BCYAN);
	ft_printf("%s", path);
	ft_printf(COLOR_BRED);
	ft_printf(" @~"COLOR_BYELLOW"=+"COLOR_BWHITE"=+"
			COLOR_BGREEN"=+>");
	ft_printf(COLOR_RESET);
}


int	main(int argc, char **argv, char **envp)
{
	t_argu		argu;
	char		*input;
	char		**lst;
	char		**tmp;
	t_inputs	*lst;
	int		i;		// ! erase later

	argu.status = 1;
	argu.argv = argv;
	argu.envp = envp;
	argu.cur_path = getcwd(NULL, 0);
	while(argu.status)
	{
		make_prompt_msg(argu);
		get_input(&input);
		printf("input test : %s\n", input);
		lst = semi_colon_split(input);
		tmp = lst;
		i = 0;
		while (*lst)
		{
			skip_space(lst);
			printf("line[%d] : %s\n", i++, *lst);
			command_branch(*lst); // 만들어진 문자리스트 && t_argu
			lst++;
		}
	}
	return (0);
}
