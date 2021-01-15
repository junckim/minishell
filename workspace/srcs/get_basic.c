/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_basic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junkang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:37:56 by junkang           #+#    #+#             */
/*   Updated: 2021/01/14 19:37:58 by junkang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int		get_index_basic(t_word_block *ret, char **ref)
{
	char	*line;
	int		i;

	line = *ref;
	i = -1;
	while (line[++i])
	{
		if (ft_isset(line[i], "\'\"") || ft_isspace(line[i]))
			break ;
		else if (is_sep(line[i]))
		{
			ret->sep = sep_to_int(line[i], line[i + 1]);
			break ;
		}
		else if (ft_isset(line[i], "\\"))
		{
			line[i] = -1;
			i++;
		}
	}
	return (i);
}

void			get_basic(t_word_block *word, char **ref)
{
	char	*line;
	int		i;

	line = *ref;
	i = get_index_basic(word, ref);
	free(word->word);
	word->word = strdup_idx(line, i);
	if (ft_isspace(line[i]) == 0 && line[i] != 0 && is_sep(line[i]) == 0)
		word->is_conti = 1;
	else
	{
		word->is_conti = 0;
		i = not_conti(word, line, i);
	}
	(*ref) += i;
	skip_space(ref);
}
