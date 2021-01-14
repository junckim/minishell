#include "../include/minishell.h"

/*
* *		환경변수가 끝나는 다음 인덱스를 반환
**		param  : 단어, $의 위치
**		return : 환경변수가 끝나는 다음 인덱스
*/
int					end_env_index(char *word, int i)
{
	while (word[++i])
	{
		if (word[i] == '.' || word[i] ==  '$' || \
			word[i] == '?' || word[i] == ':' || \
			word[i] == ' ' || word[i] == '\'' || word[i] == '=')
			break;
	}
	return (i);
}

int					end_env_bracelet(char *word, int i)
{
	while (word[++i])
	{
		if (word[i] == '}')
			break;
	}
	return (i);
}

int					env_strdup(char **word, int start, int end, char *val)
{
	int			ret;
	char		c;
	char		*new;
	char		*tmp;

	c = (*word)[start];
	(*word)[start] = 0;
	new = ft_strdup((*word));
	(*word)[start] = c;
	tmp = new;
	new = ft_strjoin(new, val);
	free(tmp);
	ret = ft_strlen(new) - 1;
	tmp = new;
	new = ft_strjoin(new, &((*word)[end]));
	free(tmp);
	free(*word);
	(*word) = new;
	return (ret);
}