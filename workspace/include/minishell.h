/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimjoochan <marvin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 15:12:06 by kimjoocha         #+#    #+#             */
/*   Updated: 2020/12/27 00:45:31 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include "../libs/libft/libft.h"
# include "../libs/libftprintf/include/ft_printf.h"

# include <stdio.h>

# define SEMI		0		// 세미콜론 or 널문자
# define PIPE		1		// | 가 들어온 경우
# define REDIR		2		// > 가 들어온 경우
# define REV_REDIR	3		// < 가 들어온 경우
# define D_REDIR	4		// >> 가 들어온 경우

# define BQU		1		// "
# define SQU		2		// '
# define BSL		3		// "\"

# define ECHO		1
# define CD			2
# define PWD		3
# define EXPORT		4
# define UNSET		5
# define ENV		6
# define LS			7
# define EXIT		8

# define COLOR_RED		"\x1b[31m"
# define COLOR_GREEN	"\x1b[32m"
# define COLOR_YELLOW	"\x1b[33m"
# define COLOR_BLUE		"\x1b[34m"
# define COLOR_MAGENTA	"\x1b[35m"
# define COLOR_CYAN		"\x1b[36m"
# define COLOR_WHITE	"\x1b[37m"
# define COLOR_BRED		"\x1b[91m"
# define COLOR_BGREEN	"\x1b[92m"
# define COLOR_BYELLOW	"\x1b[93m"
# define COLOR_BBLUE	"\x1b[94m"
# define COLOR_BMAGENTA	"\x1b[95m"
# define COLOR_BCYAN	"\x1b[96m"
# define COLOR_BWHITE	"\x1b[97m"
# define COLOR_RESET	"\x1b[0m"

typedef struct	s_env				//for envp
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct	s_pair
{
	int		fd;
	int		redir;
}				t_pair;

typedef struct	s_inputs
{
	int				sep;			// SEMI / PIPE
	int				command;		// 0 is not expected command
	char			*str;
	t_pair			*pr;
}				t_inputs;

typedef struct  s_word_block
{
    char    quotation;		//	기본 == 0 / 작은 따옴표 / 큰 따옴표
    char    *word;			//	단어 / NULL이면 더 가져올 것이 없다
	int		space_has;		//	뒤에 공백이 있는가? 0 == 문자 / 1 == 공백 / 2 == 마지막 단어
	int		sep;			//	구분자 없으면 -1;
}               t_word_block;

void	command_branch(char *command);
void	skip_space(char **str);
t_list	*split_separator(char *line, t_env *env);
int		atoi(const char *fd);
int		ft_isspace(char c);
int		ft_isset(char c, const char *set);

#endif