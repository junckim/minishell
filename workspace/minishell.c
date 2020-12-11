#include "./minishell.h"
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

void	skip_space(char **str)
{
	while (**str ==32 || (**str > 7 && **str < 13))
		(*str)++;
}

void	command_branch(char *command)
{
	if (ft_strncmp(command, "echo", 4))
		echo();
}

int	main()
{
	char	*input;
	int 	status;
	char	**lst;

	status = 1;
	while(status)
	{
		write(1, "test->", 6);
		get_input(&input);
		printf("input test : %s\n", input);
		lst = ft_split(input, ';');
		while (*lst)
		{
			skip_space(lst);
			command_branch(*lst);
			lst++;
		}

	}

	return (0);
}
