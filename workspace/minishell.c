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

int	main()
{
	char	*input;
	int 	status;

	status = 1;
	while(status)
	{
		write(1, "test->", 6);
		get_input(&input);
		printf("input test : %s\n", input);
		sleep(1);
	}

	return (0);
}
