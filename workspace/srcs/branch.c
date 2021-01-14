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

extern int	g_error_status;

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
	ret->word = ft_strdup(line);
	line[i] = tmp;
}

int		is_sep(char c)
{
	if (ft_isset(c, "|;><"))
		return (1);
	return (0);
}

char	*strdup_idx(char *line, int idx)
{
	char		*ret;
	char		tmp;
	
	tmp  = line[idx];
	line[idx] = 0;
	ret = ft_strdup(line);
	line[idx] = tmp;
	return (ret);
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

void	get_single_quotation(t_word_block *word, char **ref)
{
	char	*line;
	int		i;

	(*ref)++;
	line = *ref;
	i = -1;
	while (line[++i])
		if (line[i] == word->quotation)
			break ;
	if (line[i] == 0)
	{
		printf("Wait standard input\t-\tget_quotation\n");
		return ;
	}
	word->word = strdup_idx(line, i);		// i 자리 전까지 복사
	i++;
	if (ft_isspace(line[i]) == 0 && line[i] != 0 && is_sep(line[i]) == 0)
		word->is_conti = 1;
	else
	{
		word->is_conti = 0;
		while (ft_isspace(line[i]))
			i++;
		if (line[i] == 0)
			word->sep = 0;
		else if (is_sep(line[i]))
		{
			word->sep = sep_to_int(line[i], line[i + 1]);
			i++;
			if (word->sep == D_REDIR)
				i++;
		}
		else
			word->sep = SPACE;
	}
	(*ref) += i;
	skip_space(ref);
}

int		get_index_double(t_word_block *word, char **ref)
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
void	get_double_quotation(t_word_block *word, char **ref)
{
	char	*line;
	int		i;

	(*ref)++;
	line = *ref;
	i = get_index_double(word, ref);		// 큰 따옴표 닫히는 위치 반환
	if (line[i] == 0)
	{
		printf("Wait standard input\t-\tget_quotation\n");
		return ;
	}
	word->word = strdup_idx(line, i);
	i++;
	if (ft_isspace(line[i]) == 0 && line[i] != 0 && is_sep(line[i]) == 0)
		word->is_conti = 1;
	else
	{
		word->is_conti = 0;
		while (ft_isspace(line[i]))
			i++;
		if (line[i] == 0)
			word->sep = 0;
		else if (is_sep(line[i]))
		{
			word->sep = sep_to_int(line[i], line[i + 1]);
			i++;
			if (word->sep == D_REDIR)
				i++;
		}
		else
			word->sep = SPACE;
	}
	(*ref) += i;
	skip_space(ref);
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
void	get_basic(t_word_block *word, char **ref)
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
		while (ft_isspace(line[i]))
			i++;
		if (line[i] == 0)
			word->sep = 0;
		else if (is_sep(line[i]))
		{
			word->sep = sep_to_int(line[i], line[i + 1]);
			i++;
			if (word->sep == D_REDIR)
				i++;
		}
		else
			word->sep = SPACE;
	}
	(*ref) += i;
	skip_space(ref);
}

void	word_init(t_word_block *word)
{
	word->quotation = -1;
	word->word = ft_strdup("");
	word->is_conti = -1;
	word->sep = -1;
}

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
		if ((*ref)[0] == '\'')
			get_single_quotation(&word, ref);
		else
			get_double_quotation(&word, ref);
	}
	else
	{
		word.quotation = 0;
		get_basic(&word, ref);
	}
	return (word);
}

void	word_free(t_word_block *word)
{
	word->quotation = -1;
	free(word->word);
	word->is_conti = -1;
	word->sep = -1;
}

/*
* *		두 단어를 조인해줌. 소스는 프리.
* *		워드는 조인하고, 나머지는 뒤에 붙은 단어의 것을 상속받는다
* *		-1 처리
**		param  : 조인할 두 단어
*/
void	word_join(t_word_block *dest, t_word_block *srcs)
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

/*
* *		환경변수가 끝나는 다음 인덱스를 반환
**		param  : 단어, $의 위치
**		return : 환경변수가 끝나는 다음 인덱스
*/
int					end_env_index(char *word, int i)
{
	while (word[++i])
	{
		if (word[i] == '.' || word[i] ==  '$' || word[i] == '?' || word[i] == ':' || word[i] == ' ' || word[i] == '\'' || word[i] == '=')
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

int					isvalid_env_mark(char *word, int idx)
{
	if (idx > 0 && word[idx - 1] == -1)
		return (0);
	return (1);
}

int				change_basic(char **word, int idx, t_env *env)
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

int					change_return(char **word, int idx, t_env *env)
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

int					end_env_bracelet(char *word, int i)
{
	while (word[++i])
	{
		if (word[i] == '}')
			break;
	}
	return (i);
}

int					change_bracelet(char **word, int idx, t_env *env)
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

// echo abc > abc | anjd ;
// lst 노드 , 
// str -> 연결시켜줄 새로운 노드 객체, 
// redir -> > >> < 가 들어왔을 때 redirection으로 되야할 때 숫자로 저장
void				make_strsadd(t_commands *node, char *str, int redir)
{
	t_str	*head;
	t_str	*new;

	new = (t_str *)err_malloc(sizeof(t_str));
	new->redir = redir;
	new->word = ft_strdup(str);
	new->next = NULL;
	if (node->str == NULL)
		node->str = new;
	else
	{
		head = node->str;
		while (node->str->next)
			node->str = node->str->next;
		node->str->next = new;
		node->str = head;
	}
}

/*
* *		|; 이전까지를 하나의 노드로 만들고, 노드에 sep str 저장
*/
void				parse_node(char **ref, t_commands *node, t_env *env)
{
	t_word_block	word;
	t_word_block	part;

	word_init(&word);
	skip_space(ref);
	while ((part = get_word(ref)).word)		// < > ; | 단어 전에 띄어쓰기까지 파싱, 공백은 넘어가줌
	{
		if (part.quotation != '\'')
			change_env(&part, env);			// 환경변수 치환
		word_join(&word, &part);			// -1 처리
		if (word.is_conti == 0)
		{
			if (word.sep == SPACE)
				make_strsadd(node, word.word, -1);
			else if (word.sep == REDIR || word.sep == D_REDIR || word.sep == REV_REDIR)
			{
				if (ft_strlen(word.word) != 0)
					make_strsadd(node, word.word, -1);
				make_strsadd(node, "", word.sep);
			}
			else if (word.sep == PIPE || word.sep == SEMI)
			{
				if (ft_strlen(word.word) != 0)
					make_strsadd(node, word.word, -1);
				node->sep = word.sep;
				word_free(&word);
				break ;
			}
			else if (word.sep == 0)
			{
				make_strsadd(node, word.word, -1);
				word_free(&word);
				break ;
			}
			word_free(&word);
		}
	}
	if (node->str == NULL)
		make_strsadd(node, "", -1);
}

/*
* *		리다이렉션이 있으면 fd 저장
**		param  : str을 포함한 노드
*/

void				get_fd(t_commands *node)
{
	t_str	*before;
	t_str	*head;

	head = node->str;
	before = node->str;
	while (node->str)
	{
		node->str = node->str->next;
	}
}

/*
* *		커맨드 노드를 만듦, 초기화
**		param  : line의 주솟값, 환경변수
**		return : 노드
*/
t_commands			*make_commands_new(char **ref, t_env *env)
{
	t_commands		*node;

	node = (t_commands *)err_malloc(sizeof(t_commands));
	node->sep = -1;
	node->command = -1;
	node->str = NULL;
	node->fd = 0;
	node->fdflag = 0;
	node->pipe = NULL;
	node->prev = NULL;
	node->next = NULL;
	parse_node(ref, node, env);
	return (node);
}

t_commands			*lstlast_next(t_commands *lst)
{
	while (lst)
	{
		if (lst->next == 0)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

t_commands			*lstlast_pipe(t_commands *lst)
{
	while (lst)
	{
		if (lst->pipe == 0)
			return (lst);
		lst = lst->pipe;
	}
	return (lst);
}

// echo aaa | ppp ; bbb | mmm
// echo aaa -> sep == 2
// ppp -> sep == 1

void				commands_addback(t_commands **lst, t_commands *new)
{
	t_commands		*last_semi;
	t_commands		*last_node;
	t_commands		*tmp;

	if (new == 0 || lst == 0)
		return ;
	if (*lst == 0)
	{
		*lst = new;
		return ;
	}
	last_semi = lstlast_next(*lst);
	if (last_semi->sep == PIPE)
		last_node = lstlast_pipe(last_semi);
	else
		last_node = last_semi;
	if (last_node->sep == PIPE)
	{
		last_node->pipe = new;
	}
	else if (last_node->sep == SEMI)
		last_semi->next = new;
}

/*
* *		한 줄을 리스트의 형태로 바꿔주자.
* *		노드 (;)  노드 (;) 노드 (;) 노드 (널)
* *		(파이프) (파이프)		  (파이프)
* *		노드	  노드			   노드
* *		(널)	 (널)			  (널)
**		param  : 한 줄, 환경변수
**		return : 링크드리스트
*/
t_commands			*split_separator(char *line, t_env *env)		//	! add header
{
	t_commands		*ret;
	t_commands		*node;

	ret = 0;
	while (*line)
	{
		node = make_commands_new(&line, env);
		commands_addback(&ret, node);
	}
	return (ret);
}
