/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   branch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 15:06:27 by joockim           #+#    #+#             */
/*   Updated: 2020/12/11 15:24:51 by kimjoocha        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_isspace(char c)
{
	if (c == 32 || (c > 7 && c < 13))
		return (1);
	return (0);
}

int		ft_isset(const char *str, const char *set)
{
	char	*tmp;

	while (*set)
	{
		tmp = str;
		while (*str)
		{
			if (*set == *str)
				return (1);
			str++;	
		}
		str = tmp;
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

void	get_basic(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;
	char	tmp;

	line = *ref;
	i = -1;
	while (line[++i])
	{
		if (ft_isset(line[i], "\'\"") || ft_isspace(line[i]))
			break ;
		else if (ft_isset(line[i], "\\"))
			i++;
	}
	if (ft_isspace(line[i]))
		ret->space_has = 1;
	else if (line[i] == 0)
		ret->space_has = 2;
	tmp = line[i];
	line[i] = 0;
	free(ret->word);
	ret->word = ft_strdup(line);
	line[i] = tmp;
	(*ref) += i;
}

void	word_init(t_word_block *word)
{
	word->quotation = 0;
	word->word = ft_strdup("");
	word->space_has = 0;

}

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

void	word_join(t_word_block *dest, t_word_block *srcs)
{
	char		*tmp;

	tmp = dest->word;
	dest->word = ft_strjoin(dest->word, srcs->word);		// null이 들와
	free(tmp);
	dest->space_has = srcs->space_has;
}

void	word_free(t_word_block *word)
{
	word->quotation = 0;
	free(word->word);
	word->space_has = 0;
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

/**
* *		this line is for 'semi_colon_split' function
*
*		! means "add fuction in header"
**/

int				need_split(t_word_block word)
{
	if (word.quotation == 0 && ft_isset(word.word, ";|><"))
		return (1);
	return (0);
}

void				split_and_save_node(t_list **ret, char *buf, char *str)
{
	t_list		*lst;
	t_inputs	*node;
	int			i;

	i = -1;
	while (++i < ft_strlen(str))
	{
		if (str[i] == '>' && i != ft_strlen(str) - 1 && str[i + 1] != '>')
		{
			make_node(buf, D_REDIR)
			node.sep = D_REDIR;
			i++;
		}
		else if (str[i] == '>')
			node.sep = REDIR;
		else if (str[i] == '<')
			node.sep = REV_REDIR;
		else if (str[i] == '|')
			node.sep = PIPE;
		
	}
}

t_list				*split_separator(char *line)		//	!
{
	t_word_block		word;
	t_list				*ret;
	char				*buf;
	char				*tmp;
	char				**split;

	buf = ft_strdup("");
	while ((word = get_word(&line)).word)
	{
		if (need_split(word))
			split_and_save_node(&ret, buf, word.word);
		else
		{
			tmp = buf;
			buf = ft_strjoin(buf, word.word);
			free(tmp);
			if (word.space_has)
			{
				tmp = buf;
				buf = ft_strjoin(buf, " ");
				free(tmp);
			}
		}
	}
	return (ret);
}