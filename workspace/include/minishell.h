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

# define SYS_CMD_NOT_FOUND		127
# define SYS_SYNTAX				258

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

typedef struct	s_path
{
	char			*path;
	struct s_path	*next;
}				t_path;

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

// utils.c
void		skip_space(char **str);
int			ft_atoi(const char *fd);
int			ft_isspace(char c);
int			ft_isset(char c, const char *set);
void		*err_malloc(unsigned int n);

void		make_prompt_msg(void);
void		signal_handler(int signo);
void		signal_func(void);
t_env		*envp_to_env(char *envp);
void		add_envlst(t_env *env, char *envp);
t_env		*make_envlst(char **envp);
char		*get_value(t_env *env, char *key);
t_env		*get_env_pointer(t_env *env, char *key);
void		add_change_env(t_env *env, char *key, char *value);
void		add_own_path(t_env *env);
t_env		*set_env_lst(char **envp);

void	kill_process(int pid);
void	check_d(int	*ret, char *buf, char *str);

// input_sequence.c
int		check_input(char *str);
int		get_input(char **input);
void	slash_doing(char **input);
void	quo_doing(char **input, int quo);
void	input_sequence(char **input);

// make_path_lst.c
t_path	*new_path_one(char *str);
t_path	*add_path(t_path *path, char *str);
t_path	*make_path_lst(t_env *env);

int		strcmp_upper(const char *command, const char *str);
int		is_command(char *cmd);
int		lstsize_str(t_str *lst);
char	**str_to_argv(t_commands *node);
int		lstsize_env(t_env *lst);
char	*triple_join(char *s1, char *s2, char *s3);
char	**env_to_envp(t_env *env);

int		ft_exitstatus(int status);
int		ft_ifsignal(int status);

int		path_work(t_commands *node, char *path, t_env *env);
int		excute_work(t_commands *node, t_env *env);
int		path_excute(t_commands *node, t_env *env, t_path *path);
void	work_command(t_commands *node, t_env **env);
void	pipe_doing(t_commands *node, t_env **env);
void	start_work(t_commands *node, t_env **env);


void		command_branch(char *command);
t_commands	*split_separator(char *line, t_env *env);
char		**env_to_envp(t_env *env);
void		add_change_env(t_env *env, char *key, char *value);
char		*get_value(t_env *env, char *key);
int			list_check(t_commands *lst);
void		error_check(int err_num, char *error_message);
t_env		*get_env_pointer(t_env *env, char *key);

// free_nodes.c
void		clear_str_node(t_str **str);
void		pipe_clear(t_commands **node);
void		clear_node(t_commands **node);
void		free_all_node(t_commands **node);

int			work_redir(t_commands *node);

// -------------------------------------------------

//		command_work.c
int			command_work(t_commands *node, t_env **env, int cmd);
//		unset_work.c
int				unset_work(t_commands *node, t_env **env);
//		get_word.c
t_word_block	get_word(char **ref);
//		commands_addback.c
void		commands_addback(t_commands **lst, t_commands *new);
//		get_quotiation.c
void		get_quotation(t_word_block *word, char **ref);
//		get_basic.c
void		get_basic(t_word_block *word, char **ref);
//		word_init.c
void		word_init(t_word_block *word);
void		word_join(t_word_block *dest, t_word_block *srcs);
void		word_free(t_word_block *word);
//		get_word_utils.c
void		get_str_to_idx(t_word_block *ret, char *line, int i);
int			is_sep(char c);
char		*strdup_idx(char *line, int idx);
int			sep_to_int(char sep, char next);
int			not_conti(t_word_block *word, char *line, int i);
//		change_env.c
void		change_env(t_word_block *word, t_env *env);
//		change_env_utils.c
int			end_env_index(char *word, int i);
int			end_env_bracelet(char *word, int i);
int			env_strdup(char **word, int start, int end, char *val);
//		parse_node.c
void		parse_node(char **ref, t_commands *node, t_env *env);
void		make_strsadd(t_commands *node, char *str, int redir);


#endif
