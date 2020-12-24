#include "../include/minishell.h"

int         ctoi(const char *fd)
{
    int     ret;

    ret = 0;
    while (*fd)
    {
        ret *= 10;
        ret += (*fd) - '0';
        fd++;
    }
    return (ret);
}
/*
* *     echo 함수, str은 다 가공된 채로 들어온다
**      param  : argv[자기자신, fd, str]
**      return : 종료값
*/
int             main(int argc, char *argv[], char *envp[])
{
    char        *str;
    int         flag;

    if (argc != 3)
        return (-1);
    str = argv[2];
    flag = 0;
    while (1)
    {
        if (ft_strlen(str) > 1 && ft_strncmp(str, "-n", 2) == 0 && (*(str + 2) == 0 || ft_isspace(*(str + 2))))
        {
            str += sizeof(char) * 3;
            flag = 1;
        }
        else
            break ;
    }
    write(ctoi(argv[1]), str, ft_strlen(str));
    if (flag == 0)
        write(1, "\n", 1);
}

/*
int		ft_isspace(char c)
{
	if (c == 32 || (c > 7 && c < 13))
		return (1);
	return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (*s++)
		i++;
	return (i);
}

int     ft_strncmp(const char *str1, const char *str2, size_t n)
{
	while (n--)
	{
		if (*str1 != *str2)
			return ((unsigned char)*str1 - (unsigned char)*str2);
		else if (*str1 == 0 || *str2 == 0)
			return (0);
		str1++;
		str2++;
	}
	return (0);
}
*/