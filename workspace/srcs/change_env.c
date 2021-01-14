#include "../include/minishell.h"

extern int	g_error_status;

static int	isvalid_env_mark(char *word, int idx)
{
	if (idx > 0 && word[idx - 1] == -1)
		return (0);
	return (1);
}

static int	change_basic(char **word, int idx, t_env *env)
{
	int		j;
	char	c;
	char	*key;
	char	*val;

	j = end_env_index(*word, idx);
	c = (*word)[j];
	(*word)[j] = 0;
	key = ft_strdup(&((*word)[idx + 1]));
	(*word)[j] = c;
	if ((val = get_value(env, key)) == NULL)
		val = ft_strdup("");			// 이 경우에 프리해주긴 해야는데
	free(key);
	idx = env_strdup(word, idx, j, val);	// free(word), return 마지막 인덱스
	return (idx);
}

static int	change_return(char **word, int idx, t_env *env)
{
	int		j;
	char	c;
	char	*key;
	char	*val;

	j = idx + 2;
	c = (*word)[j];
	(*word)[j] = 0;
	key = ft_strdup(&((*word)[idx + 1]));
	(*word)[j] = c;
	if ((val = ft_itoa(g_error_status)) == NULL)
		val = ft_strdup("");			// 이 경우에 프리해주긴 해야는데
	free(key);
	idx = env_strdup(word, idx, j, val);	// free(word), return 마지막 인덱스
	return (idx);
}

static int	change_bracelet(char **word, int idx, t_env *env)
{
	int		j;
	char	c;
	char	*key;
	char	*val;

	j = end_env_bracelet(*word, idx);
	c = (*word)[j];
	(*word)[j] = 0;
	key = ft_strdup(&((*word)[idx + 2]));
	(*word)[j] = c;
	if ((val = get_value(env, key)) == NULL)
		val = ft_strdup("");			// 이 경우에 프리해주긴 해야는데
	free(key);
	idx = env_strdup(word, idx, j + 1, val);	// free(word), return 마지막 인덱스
	return (idx);
}

/*
* *		환경변수를 찾으면 치환
**		param  : 수정해줄 단어, 환경변수 목록
*/
void				change_env(t_word_block *word, t_env *env)
{
	int		i;

	i = -1;
	while ((word->word)[++i])
	{
		if ((word->word)[i] == '$' && (word->word)[i + 1] == '?')
			i = change_return(&(word->word), i, env);
		else if ((word->word)[i] == '$' && (word->word)[i + 1] == '{')
			i = change_bracelet(&(word->word), i, env);
		else if ((word->word)[i] == '$' && isvalid_env_mark(word->word, i))
			i = change_basic(&(word->word), i, env);
	}
}