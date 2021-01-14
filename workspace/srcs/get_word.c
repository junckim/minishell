/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junkang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 19:40:07 by junkang           #+#    #+#             */
/*   Updated: 2021/01/14 19:40:09 by junkang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_word_block	get_word(char **ref)
{
	t_word_block		word;

	word_init(&word);
	if ((*ref)[0] == 0)
	{
		free(word.word);
		word.word = NULL;
		return (word);
	}
	if ((*ref)[0] == '\'' || (*ref)[0] == '\"')
	{
		word.quotation = (*ref)[0];
		get_quotation(&word, ref);
	}
	else
	{
		word.quotation = 0;
		get_basic(&word, ref);
	}
	return (word);
}
