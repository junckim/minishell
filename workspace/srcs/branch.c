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
	line[i] = 0;
	free(ret->word);
	ret->word = ft_strdup(line);
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
	line[i] = 0;
	free(ret->word);
	ret->word = ft_strdup(line);
	(*ref) += (i + 1);
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
	else if (sep == ';' || ft_isspace(sep))
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
	get_str_to_idx(ret, line, i);						// 
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
	skip_space(&line);
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

void	word_join(t_word_block *dest, t_word_block *srcs)
{
	char		*tmp;

	if (dest->space_has)
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
	word_free(srcs);
}

int		cmd_echo_cmp(t_word_block command)
{
	if (ft_strlen(command.word) != 4 && ft_strlen(command.word) != 9)
		return (0);
	else if (ft_strncmp(command.word, "echo", 4) != 0 && ft_strncmp(command.word, "/bin/echo", 9))
		return (0);
	return (1);
}

void	branch_echo(char **ref)			//	어려워보이니까 나중에 짜자....
{
	printf("here is echo part\n");
}

int		cmd_cd_cmp(t_word_block command)
{
	if (ft_strlen(command.word) != 2)
		return (0);
	else if (ft_strncmp(command.word, "cd", 2) != 0)
		return (0);
	return (1);
}

void	branch_cd(char **ref)
{
	t_word_block	word;

	word = get_word(ref);
	if (chdir(word.word) != 0)
		printf("chdir error\t-\tbranch_cd\n");
	free(word.word);
}

int		cmd_pwd_cmp(t_word_block command)
{
	if (ft_strlen(command.word) != 3 && ft_strlen(command.word) != 8)
		return (0);
	else if (ft_strncmp(command.word, "pwd", 3) != 0 && ft_strncmp(command.word, "/bin/pwd", 8) != 0)
		return (0);
	return (1);
}

void	branch_pwd(void)
{
	int		size;
	char	*buf;

	size = (1 << 10);
	if (getcwd(buf, size) == 0)
	{
		write(1, strerror(errno), ft_strlen(strerror(errno)));
		return ;
	}
	write(1, buf, ft_strlen(buf));
	write(1, "\n", 1);
}

int		cmd_ls_cmp(t_word_block command)
{
	if (ft_strlen(command.word) != 2 && ft_strlen(command.word) != 7)
		return (0);
	else if (ft_strncmp(command.word, "ls", 2) != 0 && ft_strncmp(command.word, "/bin/ls", 7) != 0)
		return (0);
	return (1);
}

void	branch_ls(void)
{
	DIR				*dir_ptr;
	struct dirent	*file;
	char			buf[1 << 10];

	if (getcwd(buf, (1 << 10)) == 0)
	{
		write(1, strerror(errno), ft_strlen(strerror(errno)));
		return ;
	}
	if ((dir_ptr = opendir(buf)) == NULL)
	{
		write(1, strerror(errno), ft_strlen(strerror(errno)));
		return ;
	}
	while ((file = readdir(dir_ptr)) != NULL)
	{
		if (file->d_name[0] == '.')
			continue ;
		write(1, file->d_name, ft_strlen(file->d_name));
		write(1, "\n", 1);
	}
}

void	command_branch(char *line)
{
	t_word_block		command;
	t_word_block		next;

	word_init(&command);
	while (command.space_has == 0)		//	command part
	{
		if ((next = get_word(&line)).word == 0){
			next.word = ft_strdup("");
			next.space_has = 1;
		}
		word_join(&command, &next);
		word_free(&next);
	}
	if (cmd_echo_cmp(command) == 1)
		branch_echo(&line);
	else if (cmd_cd_cmp(command) == 1)
		branch_cd(&line);
	else if (cmd_pwd_cmp(command) == 1)
		branch_pwd();
	else if (cmd_ls_cmp(command) == 1)
		branch_ls();
	// else
		// branch_error();
}

/*
* *		under this line is for 'split_separator' function
**
**		! means "add fuction in header"
*/

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

	word_init(&string);
	while ((word = get_word(line)).word)
	{
		word_join(&string, &word);
		word_free(&word);
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
			(*str) = 0;
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

	while (*line)
	{
		content = (t_inputs *)malloc(sizeof(t_inputs));
		parse_command(&line, &(content->command), &(content->str), &(content->sep));
		ft_lstadd_back(&ret, ft_lstnew(content));
	}
	return (ret);
}
