/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junkang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:38:32 by junkang           #+#    #+#             */
/*   Updated: 2021/01/14 19:38:44 by junkang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			end_env_index(char *word, int i)
{
	while (word[++i])
	{
		if (word[i] == '.' || word[i] == '$' ||\
			word[i] == '?' || word[i] == ':' ||\
			word[i] == ' ' || word[i] == '\'' || word[i] == '=')
			break ;
	}
	return (i);
}

int			end_env_bracelet(char *word, int i)
{
	while (word[++i])
	{
		if (word[i] == '}')
			break ;
	}
	return (i);
}

int			env_strdup(char **word, int start, int end, char *val)
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
