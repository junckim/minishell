#include "../include/minishell.h"

/*
* *		기본, "", '' 파트 단위로 단어 구조체를 파싱해줌
* *		환경변수 -1 표식
* *		공백 넘기고 뒤에 나오는 sep 확인, 단어 | ; >> < > NULL
* *		sep이 있으면 is_conti = 0
**		param  : 한 줄, 사용한만큼 주소를 넘겨줄거다
**		return : 단어 구조체
*/
t_word_block	get_word(char **ref)
{
	t_word_block		word;

	word_init(&word);
	if ((*ref)[0] == 0)
	{
		free(word.word);
		word.word = NULL;
		return (word);
	}
	if ((*ref)[0] == '\'' || (*ref)[0] == '\"')
	{
		word.quotation = (*ref)[0];
		get_quotation(&word, ref);
	}
	else
	{
		word.quotation = 0;
		get_basic(&word, ref);
	}
	return (word);
}