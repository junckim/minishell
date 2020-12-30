/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   branch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 15:06:27 by joockim           #+#    #+#             */
/*   Updated: 2020/12/16 17:52:22 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_cnt_minus(const char *s)
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
char	*ft_except_strdup(const char *s)
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

void		get_str_to_idx(t_word_block *ret, char *line, int i)
{
	char		tmp;

	tmp = line[i];
	line[i] = 0;
	free(ret->word);
	ret->word = ft_except_strdup(line);
	line[i] = tmp;
}

void	get_single_quotation(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;

	(*ref)++;
	line = *ref;
	i = -1;
	while (line[++i])
		if (line[i] == ret->quotation)
			break ;
	if (line[i] == 0)
	{
		printf("Wait standard input\t-\tget_quotation\n");
		return ;
	}
	if (ft_isspace(line[i + 1]))
		ret->space_has = 1;
	else if (line[i + 1] == 0)
		ret->space_has = 2;
	get_str_to_idx(ret, line, i);
	(*ref) += (i + 1);
}

int		get_index_double(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;

	line = *ref;
	i = -1;
	while (line[++i])
	{
		if (line[i] == ret->quotation)
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
void	get_double_quotation(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;

	(*ref)++;
	line = *ref;
	i = get_index_double(ret, ref);
	if (line[i] == 0)
	{
		printf("Wait standard input\t-\tget_quotation\n");
		return ;
	}
	if (ft_isspace(line[i + 1]))
		ret->space_has = 1;
	else if (line[i + 1] == 0)
		ret->space_has = 2;
	get_str_to_idx(ret, line, i);
	(*ref) += (i + 1);
}

int		sep_to_int(char sep, char next)
{
	if (sep == '>' && next == '>')
		return (D_REDIR);
	else if (sep == '>')
		return (REDIR);
	else if (sep == '<')
		return (REV_REDIR);
	else if (sep == '|')
		return (PIPE);
	else if (sep == ';')
		return (SEMI);
	return (-1);
}

int		get_index_basic(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;

	line = *ref;
	i = -1;
	while (line[++i])
	{
		if (ft_isset(line[i], "\'\"") || ft_isspace(line[i]))
			break ;
		else if (ft_isset(line[i], "|><;") && i != (int)ft_strlen(line) - 1)
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
* *		생략해야하는 \는 -1로 치환해서 처리
**		param  : 단어 블록 원본, 라인 주솟값
*/
void	get_basic(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;

	line = *ref;
	i = get_index_basic(ret, ref);
	if (ft_isspace(line[i]))
		ret->space_has = 1;
	else if (line[i] == 0)
		ret->space_has = 2;
	get_str_to_idx(ret, line, i);
	if (ret->sep == -1 || ret->sep == REDIR || ret->sep == D_REDIR || ret->sep == REV_REDIR)
		(*ref) += i;
	else
		(*ref) += (i + 1);
}

void	word_init(t_word_block *word)
{
	word->quotation = 0;
	word->word = ft_strdup("");
	word->space_has = 0;
	word->sep = -1;
}

/*
* *		기본, "", '' 파트 단위로 단어 구조체를 파싱해줌
**		param  : 한 줄, 사용한만큼 주소를 넘겨줄거다
**		return : 단어 구조체
*/
t_word_block	get_word(char **ref)
{
	t_word_block		ret;
	char				*line;

	line = *ref;
	skip_space(ref);
	word_init(&ret);
	if (line[0] == 0)
	{
		ret.word = NULL;
		return (ret);
	}
	if (line[0] == '\'' || line[0] == '\"')
	{
		ret.quotation = line[0];
		if (line[0] == '\'')
			get_single_quotation(&ret, ref);
		else
			get_double_quotation(&ret, ref);
	}
	else
		get_basic(&ret, ref);
	skip_space(ref);
	return (ret);
}

void	word_free(t_word_block *word)
{
	word->quotation = 0;
	free(word->word);
	word->space_has = 0;
}

/*
* *		두 단어를 조인해줌. 소스는 프리.
* *		워드는 조인하고, 나머지는 뒤에 붙은 단어의 것을 상속받는다
* *		사이에 공백이 있는 경우, 공백 하나 추가
**		param  : 조인할 두 단어
*/
void	word_join(t_word_block *dest, t_word_block *srcs)
{
	char		*tmp;

	if (dest->space_has == 1)
	{
		tmp = dest->word;
		dest->word = ft_strjoin(dest->word, " ");
		free(tmp);
	}
	tmp = dest->word;
	dest->word = ft_strjoin(dest->word, srcs->word);		// null이 들와
	free(tmp);
	dest->space_has = srcs->space_has;
	dest->sep = srcs->sep;
	dest->quotation = srcs->quotation;
	word_free(srcs);
}

/*
* *		두 문자열을 비교해준다
* *		뒤에 들어오는 문자열의 대문자는 소문자로 인식한다
**		param  : 비교할 두 문자열
**		return : 두 문자열의 차이값
*/
// int					strcmp_ignore_upper(char *str1, char *str2)
// {
// 	int			i;

// 	i = 0;
// 	while (str1[i] && str2[i])
// 	{
// 		if (str1[i] - str2[i] && str1[i] - (str2[i] - 'A' + 'a'))
// 			return (str1[i] - str2[i]);
// 		i++;
// 	}
// 	return (str1[i] - str2[i]);
// }

/*
* *		스트링을 읽고 어떤 cmd인지 해당 번호를 반환
* *		워드를 프리해줌
**		param  : 커맨드 문자열
**		return : 디파인된 커맨드 넘버
*/
// int					cmd_to_int(char *cmd)
// {
// 	int		ret;

// 	ret = -1;
// 	if (ft_strlen(cmd) == 4 && strcmp_ignore_upper("echo", cmd) == 0)
// 		ret = ECHO;
// 	else if (ft_strlen(cmd) == 2 && ft_strncmp(cmd, "cd", 2) == 0)
// 		ret = CD;
// 	else if  (ft_strlen(cmd) == 3 && strcmp_ignore_upper("pwd", cmd) == 0)
// 		ret = PWD;
// 	else if (ft_strlen(cmd) == 6 && ft_strncmp(cmd, "export", 6) == 0)
// 		ret = EXPORT;
// 	else if (ft_strlen(cmd) == 5 && ft_strncmp(cmd, "unset", 5) == 0)
// 		ret = UNSET;
// 	else if (ft_strlen(cmd) == 3 && strcmp_ignore_upper("env", cmd) == 0)
// 		ret = ENV;
// 	else if (ft_strlen(cmd) == 2 && strcmp_ignore_upper("ls", cmd) == 0)
// 		ret = LS;
// 	else if (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "exit", 4) == 0)
// 		ret = EXIT;
// 	free(cmd);
// 	return (ret);
// }

/*
* *		환경변수가 끝나는 다음 인덱스를 반환
**		param  : 단어, $의 위치
**		return : 환경변수가 끝나는 다음 인덱스
*/
int					end_env_index(char *word, int i)
{
	while (word[++i])
	{
		if (word[i] == '.' || word[i] ==  '$' || word[i] == '?')
			break;
	}
	return (i);
}

char				*find_env(char *find, t_env *env)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (ft_strlen(cur->key) == ft_strlen(find) && ft_strncmp(cur->key, find, ft_strlen(find)) == 0)
		{
			free(find);
			return (ft_strdup(cur->value));
		}
		cur = cur->next;
	}
	free(find);
	return (ft_strdup(""));
}

int					env_strdup(char **word, int start, int end, char *val)
{
	int			ret;
	char		tmp;
	char		*new;
	char		*ttmp;
	char		*tttmp;

	tmp = (*word)[start];
	(*word)[start] = 0;
	new = ft_strdup((*word));
	(*word)[start] = tmp;
	ttmp = new;
	new = ft_strjoin(new, val);
	free(val);
	free(ttmp);
	ret = ft_strlen(new) - 1;
	ttmp = new;
	new = ft_strjoin(new, (tttmp = ft_strdup(&((*word)[end]))));
	free(ttmp);
	free(tttmp);
	free(*word);
	(*word) = new;
	return (ret);
}

/*
* *		환경변수를 찾으면 치환
**		param  : 수정해줄 단어, 환경변수 목록
*/
void				change_env(t_word_block *word, t_env *env)
{
	int		i;
	int		j;
	char	tmp;
	char	*find;

	i = -1;
	while ((word->word)[++i])
	{
		if ((word->word)[i] == '$' && (word->word)[i + 1] != 0)
		{
			// if ((word->word)[i + 1] == '?')
			j = end_env_index(word->word, i);
			tmp = (word->word)[j];
			(word->word)[j] = 0;
			find = ft_strdup(&((word->word)[i + 1]));
			(word->word)[j] = tmp;
			find = find_env(find, env);					// free(find);
			i = env_strdup(&(word->word), i, j, find);	// free(word->word), free(find), return 마지막 인덱스
		}
	}
}

/*
* *		스트링에 있는 워드 이름의 파일을 생성하고 fd를 반환
* *		> 빈파일로 일단 만듦
**		param  : sep이 닮긴 pr, 스트링
*/
// void				make_fd(t_pair *pr, t_word_block string)
// {
// 	pr->fd = open(string.word, O_CREAT | O_RDWR | O_APPEND | O_EXCL);
// }

/*
* *		리다이렉션을 처리해서 반환
* *		> 파일을 만들고 초기화
**		param  : 리다이렉션이 처음에 나온는 라인
**		return : 없으면 널
*/
//		!		36 lines
// t_pair				*is_redir(char **ref, t_env *env)
// {
// 	t_pair			*pr;
// 	t_word_block	word;
// 	t_word_block	string;

// 	word_init(&string);
// 	skip_space(ref);
// 	pr = (t_pair *)malloc(sizeof(t_pair));
// 	while ((*ref)[0] != 0 && (pr->redir = sep_to_int((*ref)[0], (*ref)[1])) != -1)
// 	{
// 		if (pr->redir == SEMI || pr->redir == PIPE)
// 		{
// 			free(pr);
// 			pr = 0;
// 			break ;
// 		}
// 		else if (pr->redir == D_REDIR)
// 			(*ref) += 2;
// 		else
// 			(*ref) += 1;
// 		skip_space(ref);
// 		while ((word = get_word(ref)).word)
// 		{
// 			if (word.quotation != '\'')
// 				change_env(&word, env);
// 			word_join(&string, &word);
// 			if (string.sep != -1 || string.space_has != 0)
// 				break ;
// 		}
// 		make_fd(pr, string);
// 		if (string.sep == SEMI || string.sep == PIPE || string.sep == -1)
// 			break ;
// 		free(pr);
// 		pr = 0;
// 		skip_space(ref);
// 	}
// 	return (pr);
// }

/*
* *		커맨드는 저장을 했다. 남은 문자열로 str, sep을 저장하자
**		param  : 남은 줄, content
*/
// t_pair				*get_str_and_sep(char **line, t_inputs **content, t_env *env)
// {
// 	t_word_block	string;
// 	t_word_block	word;
// 	t_pair			*pr;

// 	skip_space(line);
// 	pr = is_redir(line, env);
// 	word_init(&string);
// 	while ((word = get_word(line)).word)
// 	{
// 		if (word.quotation != '\'')
// 			change_env(&word, env);
// 		word_join(&string, &word);
// 		if (string.sep == REDIR || string.sep == D_REDIR || string.sep == REV_REDIR)
// 		{
// 			string.sep = -1;
// 			free(pr);
// 			pr = is_redir(line, env);
// 		}
// 		if (string.sep == SEMI || string.sep == PIPE)
// 			break ;
// 	}
// 	(*content)->sep = string.sep;
// 	(*content)->str = string.word;
// 	return (pr);
// }

/*
* *		줄을 받아다 inputs 구조체의 요소들을 저장, 줄은 넘어가면서 사용
**		param  : 줄, 커맨드, 스트링, 구분자
*/
//		!		26 lines
// t_pair				*parse_command(char **line, t_inputs **content, t_env *env)
// {
// 	t_word_block	cmd;
// 	t_word_block	word;
// 	t_pair			*pr;

// 	pr = 0;
// 	word_init(&cmd);
// 	while ((word = get_word(line)).word)															// ? 구분자까지만 파싱, 리다이렉션은 냅둔다
// 	{
// 		if (word.quotation != '\'')
// 			change_env(&word, env);
// 		word_join(&cmd, &word);
// 		if (cmd.sep == PIPE || cmd.sep == SEMI)														// 커맨드 얻고 끝나는 경우
// 		{
// 			(*content)->command = cmd_to_int(cmd.word);												// ? 안에서 free 해주자
// 			(*content)->str = ft_strdup("");
// 			(*content)->sep = cmd.sep;
// 			break ;
// 		}
// 		if (cmd.space_has || cmd.sep == REDIR || cmd.sep == D_REDIR || cmd.sep == REV_REDIR)		// 커맨드를 얻고 스트링을 얻어야하는 경우
// 		{
// 			(*content)->command = cmd_to_int(cmd.word);
// 			pr = get_str_and_sep(line, content, env);
// 			break ;
// 		}
// 	}
// 	return (pr);
// }

void				word_add_space(t_word_block *cmd)
{

}


/*
* *		노드에 필요한 정보 저장
*/
void				parse_node(char **ref, t_commands *node, t_env *env)
{
	char			*line;
	t_word_block	cmd;
	t_word_block	word;
	int				flag = 0;		// 커맨드 저장!

	word_init(&cmd);
	while ((word = get_word(ref)).word)
	{
		if (word.quotation != '\'')
			change_env(&word, env);			// 환경변수 치환
		printf("word : %s\nline : %s\n", word.word, *ref);
		word_join(&cmd, &word);
		if (cmd.space_has == 1)
			word_add_space(&cmd);
		if (cmd.sep == PIPE || cmd.sep == SEMI)
			break ;
		if (**ref == '>' || **ref == '<')
			work_redir(ref, node);
	}
	node->str = cmd.word;
}

t_commands			*make_commands_new(char **ref, t_env *env)
{
	t_commands		*ret;

	ret = (t_commands *)err_malloc(sizeof(t_commands));
	parse_node(ref, ret, env);
	return (ret);
}

/*
* *		한 줄을 리스트의 형태로 바꿔주자. 구분자는 | < > >> 이다.
**		param  : 한 줄
**		return : 리스트
*/
t_commands			*split_separator(char *line, t_env *env)		//	! add header
{
	t_commands		*ret;
	t_commands		*node;

	ret = make_commands_new(&line, env);
	// while (*line)
	// 	add_commandslst(ret, &line, env);
	return (ret);
}