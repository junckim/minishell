/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kimjoochan <marvin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 15:12:06 by kimjoocha         #+#    #+#             */
/*   Updated: 2020/12/30 14:19:32 by joockim          ###   ########.fr       */
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

# define SEMI		1		// 세미콜론 or 널문자
# define PIPE		2		// | 가 들어온 경우
# define REDIR		3		// > 가 들어온 경우
# define REV_REDIR	4		// < 가 들어온 경우
# define D_REDIR	5		// >> 가 들어온 경우
# define SPACE		6		// 띄어쓰기

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

# define BUFSIZE	1000000

# define ERR_EXPORT				-2
# define ERR_EMPTY_SEMI			-3
# define ERR_EMPTY_SEMI_S		"syntax error near unexpected token `;'"
# define ERR_EMPTY_PIPE			-4
# define ERR_EMPTY_PIPE_S		"syntax error near unexpected token `|'"
# define ERR_EMPTY_REDIR		-5
# define ERR_EMPTY_REDIR_S		"syntax error near unexpected token `>'"
# define ERR_EMPTY_D_REDIR		-6
# define ERR_EMPTY_D_REDIR_S	"syntax error near unexpected token `>>'"
# define ERR_EMPTY_REV_REDIR	-7
# define ERR_EMPTY_REV_REDIR_S	"syntax error near unexpected token `<'"
# define ERR_EMPTY_NEWLINE		-8
# define ERR_EMPTY_NEWLINE_S	"syntax error near unexpected token `newline'"

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

typedef struct s_str
{
	int		redir;
	char	*word;
	struct s_str *next;
}				t_str;

typedef struct	s_commands
{
	int						sep;			// SEMI / PIPE
	int						command;		// 0 is not expected command
	t_str					*str;
	int						fd;			//	read 0 write 1
	int						fdflag;
	struct s_commands		*pipe;
	struct s_commands		*prev;
	struct s_commands		*next;			// by semicolon
}				t_commands;

typedef struct  s_word_block
{
    char    quotation;		//	기본 == 0 / 작은 따옴표 / 큰 따옴표
    char    *word;			//	단어 / NULL이면 더 가져올 것이 없다
	int		is_conti;		//	1이면 붙여쓰기
	int		sep;			//	구분자(|;><>>공백NULL) 없으면 -1;
}               t_word_block;

void		command_branch(char *command);
void		skip_space(char **str);
t_commands	*split_separator(char *line, t_env *env);
int			ft_atoi(const char *fd);
int			ft_isspace(char c);
int			ft_isset(char c, const char *set);
void		*err_malloc(unsigned int n);
char		**env_to_envp(t_env *env);
int			cd_work(t_commands *node, t_env *env);
int			export_work(t_commands *node, t_env *env);
int			unset_work(t_commands *node, t_env **env);
void		exit_work(t_commands *node, t_env *env);
int			command_work(t_commands *node, t_env **env, int cmd);
void		add_change_env(t_env *env, char *key, char *value);
char		*get_value(t_env *env, char *key);
int			list_check(t_commands *lst);
void		error_check(int err_num, char *error_message);
t_env		*get_env_pointer(t_env *env, char *key);
void		make_strsadd(t_commands *node, char *str, int redir);
int			work_redir(t_commands *node);

#endif
