/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 22:11:19 by joockim           #+#    #+#             */
/*   Updated: 2020/12/31 18:24:31 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	make_prompt_msg(void)
{
	char	*path;
	char	*last;

	path = getcwd(NULL, 0);
	last = ft_strrchr(path, '/');
	ft_printf(COLOR_MAGENTA);
	ft_printf("dir :");
	ft_printf(COLOR_BCYAN);
	ft_printf("%s", last);
	ft_printf(COLOR_BRED);
	ft_printf(" @~"COLOR_BYELLOW"=+"COLOR_BWHITE"=+"
			COLOR_BGREEN"=+>");
	ft_printf(COLOR_RESET);
}

void	signal_handler(int signo)
{
	signo = 0;
	write(1, "\n", 1);
	make_prompt_msg();
}

t_env	*envp_to_env(char *envp)
{
	t_env	*env;
	char	*path;
	char	*key;
	char	*value;

	path = ft_strchr(envp, '=');
	key = envp;
	value = path + 1;
	*path = 0;
	if ((env = malloc(sizeof(t_env))) == 0)
		return (0);
	env->key = ft_strdup(key);
	env->value = ft_strdup(value);
	env->next = NULL;
	return (env);
}

void	add_envlst(t_env *env, char *envp)
{
	t_env	*elem;

	elem = envp_to_env(envp);
	while (env->next)
		env = env->next;
	env->next = elem;
}

t_env	*make_envlst(char **envp)
{
	t_env	*env;

	env = envp_to_env(*envp);
	envp++;
	while (*envp)
	{
		add_envlst(env, *envp);
		envp++;
	}
	return (env);
}

char	*get_value(t_env *env, char *key)
{
	char	*value;

	value = NULL;
	while (env)
	{
		if (!ft_strncmp(key, env->key, ft_strlen(key)) &&
				!ft_strncmp(key, env->key, ft_strlen(env->key)))
			value = env->value;
		env = env->next;
	}
	return (value);
}

t_env	*get_env_pointer(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strncmp(key, env->key, ft_strlen(key)) &&
				!ft_strncmp(key, env->key, ft_strlen(env->key)))
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	add_change_env(t_env *env, char *key, char *value)
{
	t_env	*cur_env;
	
	if ((cur_env = get_env_pointer(env, key)))
	{
		free(cur_env->value);
		cur_env->value = ft_strdup(value);
	}
	else
	{
		while (env->next)
			env = env->next;
		cur_env = (t_env *)err_malloc(sizeof(t_env));
		cur_env->key = ft_strdup(key);
		cur_env->value = ft_strdup(value);
		cur_env->next  = NULL;
		env->next = cur_env;
	}
}

int		check_input(char *str)
{
	int	flag;

	flag = 0;
	while (*str)
	{
		if (*str == '\\' && *(str + 1) != 0)
			str++;
		else if (*str == '"' && flag == 0)
			flag = BQU;
		else if (*str == '\'' && flag == 0)
			flag = SQU;
		else if (*str == '"' && flag == 1)
			flag = 0;
		else if (*str == '\'' && flag == 2)
			flag = 0;
		else if (*str == '\\' && flag == 0 && *(str + 1) == 0)
			flag = BSL;
		str++;
	}
	return (flag);
}

int		get_input(char **input)
{
	int		ret;
	char	buf[2];
	char	*str;
	char	*temp;

	buf[1] = 0;
	str = ft_strjoin("", "");
	while ((ret = read(0, buf, 1)) && buf[0] != '\n')
	{
		if (buf[0] != '\n' && ret != 0)
		{
			temp = ft_strjoin(str, buf);
			free(str);
			str = temp;
		}
	}
	*input = str;
	return (check_input(str));
}

void	slash_doing(char **input)
{
	int		flag;
	char	*tmp;
	char	*more;

	write(1, ">", 1);
	tmp = ft_substr(*input, 0, ft_strlen(*input) - 1);
	free(*input);
	flag = get_input(&more);
	*input = ft_strjoin(tmp, more);
	free(more);
	free(tmp);
	if (flag == BSL)
		slash_doing(input);
}

void	quo_doing(char **input, int quo)
{
	int		flag;
	char	*temp;
	char	*more;

	if (quo == SQU)
		write(1, "quote>", 6);
	else
		write(1, "D_quote>", 8);
	temp = ft_strjoin(*input, "\n");
	free(*input);
	flag = get_input(&more);
	*input = ft_strjoin(temp, more);
	free(more);
	free(temp);
	if (flag != quo)
		quo_doing(input, quo);
}

void	input_sequence(char **input)
{
	int	flag;

	flag = get_input(input);
	if (flag == BSL)
		slash_doing(input);
	else if (flag == SQU)
		quo_doing(input, SQU);
	else if (flag == BQU)
		quo_doing(input, BQU);
}

int	main(int argc, char **argv, char **envp)
{
	int				status;
	char			*input;
	t_env			*env;
	t_commands		*lst;
	t_commands		*cur;

	argc = 0; argv = 0;
	signal(SIGINT, (void *)signal_handler);
	status = 1;
	env = make_envlst(envp);
	while(status)
	{
		make_prompt_msg();
		input_sequence(&input);
		printf("input test : %s\n", input);
		lst = split_separator(input, env);
		cur = lst;
		while (cur)
		{
			cur = cur->next;
		}
	}
	return (0);
}
