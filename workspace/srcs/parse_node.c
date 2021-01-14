#include "../include/minishell.h"

void		make_strsadd(t_commands *node, char *str, int redir)
{
	t_str	*head;
	t_str	*new;

	new = (t_str *)err_malloc(sizeof(t_str));
	new->redir = redir;
	new->word = ft_strdup(str);
	new->next = NULL;
	if (node->str == NULL)
		node->str = new;
	else
	{
		head = node->str;
		while (node->str->next)
			node->str = node->str->next;
		node->str->next = new;
		node->str = head;
	}
}

static int	save_node(t_commands *node, t_word_block *word)
{
	if (word->sep == SPACE)
		make_strsadd(node, word->word, -1);
	else if (word->sep == REDIR || word->sep == D_REDIR || \
			word->sep == REV_REDIR)
	{
		if (ft_strlen(word->word) != 0)
			make_strsadd(node, word->word, -1);
		make_strsadd(node, "", word->sep);
	}
	else if (word->sep == PIPE || word->sep == SEMI)
	{
		if (ft_strlen(word->word) != 0)
			make_strsadd(node, word->word, -1);
		node->sep = word->sep;
		word_free(word);
		return (1);
	}
	else if (word->sep == 0)
	{
		make_strsadd(node, word->word, -1);
		word_free(word);
		return (1);
	}
	word_free(word);
	return (0);
}

void		parse_node(char **ref, t_commands *node, t_env *env)
{
	t_word_block	word;
	t_word_block	part;

	word_init(&word);
	skip_space(ref);
	while ((part = get_word(ref)).word)
	{
		if (part.quotation != '\'')
			change_env(&part, env);
		word_join(&word, &part);
		if (word.is_conti == 0)
		{
			if (1 == save_node(node, &word))
				break ;
		}
	}
	if (node->str == NULL)
		make_strsadd(node, "", -1);
}