/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_word_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junkang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:38:05 by junkang           #+#    #+#             */
/*   Updated: 2021/01/14 19:38:06 by junkang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_str_to_idx(t_word_block *ret, char *line, int i)
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

	tmp = line[idx];
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

int		not_conti(t_word_block *word, char *line, int i)
{
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
	return (i);
}
