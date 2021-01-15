/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junkang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:35:51 by junkang           #+#    #+#             */
/*   Updated: 2021/01/14 19:40:16 by junkang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		word_init(t_word_block *word)
{
	word->quotation = -1;
	word->word = ft_strdup("");
	word->is_conti = -1;
	word->sep = -1;
}

static int	ft_cnt_minus(const char *s)
{
	int		ret;

	ret = 0;
	while (*s)
	{
		if (*s == -1)
			ret++;
		s++;
	}
	return (ret);
}

static char	*ft_except_strdup(const char *s)
{
	size_t	len;
	char	*res;
	char	*temp;

	len = ft_strlen(s) - ft_cnt_minus(s);
	if ((res = (char *)malloc(sizeof(char) * (len + 1))) == NULL)
		return (NULL);
	temp = res;
	while (*s)
	{
		if (*s == -1)
		{
			s++;
			continue ;
		}
		*temp++ = *s++;
	}
	*temp = 0;
	return (res);
}

void		word_join(t_word_block *dest, t_word_block *srcs)
{
	char		*tmp;

	tmp = srcs->word;
	srcs->word = ft_except_strdup(srcs->word);
	free(tmp);
	tmp = dest->word;
	dest->word = ft_strjoin(dest->word, srcs->word);
	free(tmp);
	dest->quotation = srcs->quotation;
	dest->is_conti = srcs->is_conti;
	dest->sep = srcs->sep;
	word_free(srcs);
}

void		word_free(t_word_block *word)
{
	word->quotation = -1;
	free(word->word);
	word->is_conti = -1;
	word->sep = -1;
}
