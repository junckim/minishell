/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joockim <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 22:11:19 by joockim           #+#    #+#             */
/*   Updated: 2021/01/15 18:22:44 by joockim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*g_read_str;
int		g_error_status;

static void	in_child_process(t_commands *node, int p1[2], t_env **env)
{
	if (node->pipe)
	{
		close(p1[0]);
		dup2(p1[1], STDOUT_FILENO);
	}
	if (node->fd != STDIN_FILENO && dup2(node->fd, STDIN_FILENO) < 0)
		exit(0);
	work_command(node, env);
	exit(0);
}

void		pipe_doing(t_commands *node, t_env **env)
{
	int		p1[2];
	pid_t	pid;
	int		status;

	while (node)
	{
		if (node->pipe)
		{
			pipe(p1);
			node->pipe->fd = p1[0];
		}
		if (!(pid = fork()))
			in_child_process(node, p1, env);
		else
		{
			if (node->pipe)
				close(p1[1]);
			if (node->fd != STDIN_FILENO)
				close(node->fd);
			waitpid(pid, &status, 0);
		}
		node = node->pipe;
	}
}

void		start_work(t_commands *node, t_env **env)
{
	while (node)
	{
		if (node->pipe)
		{
			pipe_doing(node, env);
		}
		else
		{
			work_command(node, env);
		}
		node = node->next;
	}
}

int			main(int argc, char **argv, char **envp)
{
	char			*input;
	t_env			*env;
	t_commands		*node;
	int				err_num;

	argc = 0;
	env = set_env_lst(envp);
	signal_func();
	while (1)
	{
		make_prompt_msg();
		input_sequence(&input);
		printf("input test : %s\n", input);
		node = split_separator(input, env);
		if ((err_num = list_check(node)) < 0)
		{
			error_check(err_num, "");
			g_error_status = SYS_SYNTAX;
		}
		else
			start_work(node, &env);
		free_all_node(&node);
	}
	return (0);
}
