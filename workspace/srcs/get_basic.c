#include "../include/minishell.h"

static int		get_index_basic(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;

	line = *ref;
	i = -1;
	while (line[++i])
	{
		if (ft_isset(line[i], "\'\"") || ft_isspace(line[i]))
			break ;
		else if (is_sep(line[i]))
		{
			ret->sep = sep_to_int(line[i], line[i + 1]);
			break ;
		}
		else if (ft_isset(line[i], "\\"))
		{
			line[i] = -1;
			i++;
		}
	}
	return (i);
}

/*
* *		따옴표가 아닌 부분을 파싱
* *		구분자가 나오기 전까지 파싱
* *		생략해야하는 \는 -1로 치환해서 저장
**		param  : 단어 블록 원본, 라인 주솟값
*/
void			get_basic(t_word_block *word, char **ref)
{
	char	*line;
	int		i;

	line = *ref;
	i = get_index_basic(word, ref);		// 끝난 다음의 위치
	word->word = strdup_idx(line, i);
	if (ft_isspace(line[i]) == 0 && line[i] != 0 && is_sep(line[i]) == 0)
		word->is_conti = 1;
	else
	{
		word->is_conti = 0;
		i = not_conti(word, line, i);
	}
	(*ref) += i;
	skip_space(ref);
}