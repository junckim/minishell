#include "../include/minishell.h"

void		word_init(t_word_block *word)
{
	word->quotation = -1;
	word->word = ft_strdup("");
	word->is_conti = -1;
	word->sep = -1;
}

static int	ft_cnt_minus(const char *s)
{
	int		ret;

	ret = 0;
	while (*s)
	{
		if (*s == -1)
			ret++;
		s++;
	}
	return (ret);
}

/*
* *		문자열 중간에 삽입된 -1을 삭제
**		param  : 문자열 원본
**		return : 가공된 문자열
*/
static char	*ft_except_strdup(const char *s)
{
	size_t	len;
	char	*res;
	char	*temp;

	len = ft_strlen(s) - ft_cnt_minus(s);
	if ((res = (char *)malloc(sizeof(char) * (len + 1))) == NULL)
		return (NULL);
	temp = res;
	while (*s)
	{
		if (*s == -1)
		{
			s++;
			continue ;
		}
		*temp++ = *s++;
	}
	*temp = 0;
	return (res);
}

/*
* *		두 단어를 조인해줌. 소스는 프리.
* *		워드는 조인하고, 나머지는 뒤에 붙은 단어의 것을 상속받는다
* *		-1 처리
**		param  : 조인할 두 단어
*/
void		word_join(t_word_block *dest, t_word_block *srcs)
{
	char		*tmp;

	tmp = srcs->word;
	srcs->word = ft_except_strdup(srcs->word);
	free(tmp);
	tmp = dest->word;
	dest->word = ft_strjoin(dest->word, srcs->word);		// null이 들와
	free(tmp);
	dest->quotation = srcs->quotation;
	dest->is_conti = srcs->is_conti;
	dest->sep = srcs->sep;
	word_free(srcs);
}

void		word_free(t_word_block *word)
{
	word->quotation = -1;
	free(word->word);
	word->is_conti = -1;
	word->sep = -1;
}