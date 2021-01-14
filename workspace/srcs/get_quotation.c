#include "../include/minishell.h"

static void	get_single_quotation(t_word_block *word, char **ref)
{
	char	*line;
	int		i;

	(*ref)++;
	line = *ref;
	i = -1;
	while (line[++i])
		if (line[i] == word->quotation)
			break ;
	word->word = strdup_idx(line, i);		// i 자리 전까지 복사
	i++;
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

static int	get_index_double(t_word_block *word, char **ref)
{
	char	*line;
	int		i;

	line = *ref;
	i = -1;
	while (line[++i])
	{
		if (line[i] == word->quotation)
			break ;
		else if (line[i] == '\\' && (line[i + 1] == '\\' || line[i + 1] == '\"' || line[i + 1] == '$'))
		{
			line[i] = -1;
			i++;
		}
	}
	return (i);
}

/*
* *		큰따옴표로 묶인 단어 파싱
* *		생략해야하는 \의 경우, 그 자리에 -1로 바꿔서 저장
**		param  : 필요한 정보를 저장할 구조체, 원본 글
*/
static void	get_double_quotation(t_word_block *word, char **ref)
{
	char	*line;
	int		i;

	(*ref)++;
	line = *ref;
	i = get_index_double(word, ref);		// 큰 따옴표 닫히는 위치 반환
	word->word = strdup_idx(line, i);
	i++;
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

void		get_quotation(t_word_block *word, char **ref)
{
	if ((*ref)[0] == '\'')
		get_single_quotation(word, ref);
	else
		get_double_quotation(word, ref);
}