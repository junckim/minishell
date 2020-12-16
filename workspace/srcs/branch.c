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

int		ft_isspace(char c)
{
	if (c == 32 || (c > 7 && c < 13))
		return (1);
	return (0);
}

int		ft_isset(const char c, const char *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
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

void	get_double_quotation(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;

	(*ref)++;
	line = *ref;
	i = -1;
	while (line[++i])
	{
		if (line[i] == ret->quotation)
			break ;
		else if (ft_isset(line[i], "\\"))
			i++;
	}
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

void	get_basic(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;

	line = *ref;
	i = -1;
	while (line[++i])
	{
		if (ft_isset(line[i], "\'\""))
			break ;
		else if (ft_isset(line[i], "|><;") && i != ft_strlen(line) - 1)
		{
			ret->sep = sep_to_int(line[i], line[i + 1]);
			break ;
		}
		else if ((ft_isset(line[i], "|><;") && i == ft_strlen(line) - 1) || ft_isspace(line[i]))
		{
			ret->sep = sep_to_int(line[i], 0);
			break ;
		}
		else if (ft_isset(line[i], "\\"))
			i++;
	}
	if (ft_isspace(line[i]))
		ret->space_has = 1;
	else if (line[i] == 0)
		ret->space_has = 2;
	get_str_to_idx(ret, line, i);
	if (line[i] == '>' && i != ft_strlen(line) && line[i + 1] == '>')
		i++;
	if (ret->sep == -1)
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
	dest->fd = srcs->fd;
	word_free(srcs);
}

/*
* *		스트링을 읽고 어떤 cmd인지 해당 번호를 반환
**		param  : 커맨드 문자열
**		return : 디파인된 커맨드 넘버
*/
int					cmd_to_int(char *cmd)
{
	if (ft_strlen(cmd) == 4 && (ft_strncmp(cmd, "echo", 4) == 0 || ft_strncmp(cmd, "Echo", 4) == 0 ||
								ft_strncmp(cmd, "eCho", 4) == 0 || ft_strncmp(cmd, "ecHo", 4) == 0 ||
								ft_strncmp(cmd, "echO", 4) == 0 || ft_strncmp(cmd, "ECho", 4) == 0 ||
								ft_strncmp(cmd, "EcHo", 4) == 0 || ft_strncmp(cmd, "EchO", 4) == 0 ||
								ft_strncmp(cmd, "eCHo", 4) == 0 || ft_strncmp(cmd, "eChO", 4) == 0 ||
								ft_strncmp(cmd, "ecHO", 4) == 0 || ft_strncmp(cmd, "ECHo", 4) == 0 ||
								ft_strncmp(cmd, "EChO", 4) == 0 || ft_strncmp(cmd, "EcHO", 4) == 0 ||
								ft_strncmp(cmd, "eCHO", 4) == 0 || ft_strncmp(cmd, "ECHO", 4) == 0))
		return (ECHO);
	else if (ft_strlen(cmd) == 2 && ft_strncmp(cmd, "cd", 2) == 0)
		return (CD);
	else if  (ft_strlen(cmd) == 3 && (ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "Pwd", 3) == 0 ||
									  ft_strncmp(cmd, "pWd", 3) == 0 || ft_strncmp(cmd, "pwD", 3) == 0 ||
									  ft_strncmp(cmd, "PWd", 3) == 0 || ft_strncmp(cmd, "PwD", 3) == 0 ||
									  ft_strncmp(cmd, "pWD", 3) == 0 || ft_strncmp(cmd, "PWD", 3) == 0))
		return (PWD);
	else if (ft_strlen(cmd) == 6 && ft_strncmp(cmd, "export", 6) == 0)
		return (EXPORT);
	else if (ft_strlen(cmd) == 5 && ft_strncmp(cmd, "unset", 5) == 0)
		return (UNSET);
	else if (ft_strlen(cmd) == 3 && (ft_strncmp(cmd, "env", 3) == 0 || ft_strncmp(cmd, "Env", 3) == 0 ||
									 ft_strncmp(cmd, "eNv", 3) == 0 || ft_strncmp(cmd, "enV", 3) == 0 ||
									 ft_strncmp(cmd, "ENv", 3) == 0 || ft_strncmp(cmd, "EnV", 3) == 0 ||
									 ft_strncmp(cmd, "eNV", 3) == 0 || ft_strncmp(cmd, "ENV", 3) == 0))
		return (ENV);
	else if (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "exit", 4) == 0)
		return (EXIT);
	return (-1);
}

/*
* *		커맨드는 저장을 했다. 남은 문자열로 str, sep을 저장하자
**		param  : 남은 줄, input->str, input->sep
*/
void				get_str_and_sep(char **line, char **str, int *sep)
{
	t_word_block	string;
	t_word_block	word;

	skip_space(line);
	word_init(&string);
	while ((word = get_word(line)).word)		// 무한 루프
	{
		word_join(&string, &word);
		if (string.sep != -1)
			break ;
	}
	(*sep) = string.sep;
	(*str) = string.word;
}

/*
* *		줄을 받아다 inputs 구조체의 요소들을 저장, 줄은 넘어가면서 사용
**		param  : 줄, 커맨드, 스트링, 구분자
*/
void				parse_command(char **line, int *command, char **str, int *sep)
{
	t_word_block	cmd;
	t_word_block	word;
	char			*tmp;

	word_init(&cmd);
	while ((word = get_word(line)).word)				// ? 구분자까지만 파싱
	{
		if (word.sep != -1)								// 구분자가 나온 경우
		{
			word_join(&cmd, &word);						// ? 뒤에 매개변수 free
			(*command) = cmd_to_int(cmd.word);			// ? 안에서 free 해주자
			(*str) = ft_strdup("");
			(*sep) = cmd.sep;
			break ;
		}
		word_join(&cmd, &word);
		if (cmd.quotation == 0 && cmd.space_has)		// 띄어쓰기가 나온 경우
		{
			(*command) = cmd_to_int(cmd.word);
			get_str_and_sep(line, str, sep);
			break ;
		}
	}
}

/*
* *		한 줄을 리스트의 형태로 바꿔주자. 구분자는 | < > >> 이다.
**		param  : 한 줄
**		return : 리스트
*/
t_list				*split_separator(char *line)		//	!
{
	t_list			*ret;
	t_inputs		*content;
	t_list			*tmp;

	ret = (t_list *)malloc(sizeof(t_list));
	while (*line)
	{
		content = (t_inputs *)malloc(sizeof(t_inputs));
		parse_command(&line, &(content->command), &(content->str), &(content->sep));
		printf("==content==\ncommand : %d\nstr : %s\nsep : %d\n", content->command, content->str, content->sep);
		// printf("before ft_lstadd_back\n");
		tmp = ft_lstnew(content);
		if (tmp == 0)
			printf("tmp is null\n");
		else
			ft_lstadd_back(&ret, tmp);
		// printf("after ft_lstadd_back\n");
	}
	return (ret);
}
