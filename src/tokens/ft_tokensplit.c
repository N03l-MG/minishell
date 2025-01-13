/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokensplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 08:26:25 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/13 15:38:47 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **ptr, int x)
{
	while (x >= 0)
	{
		if (ptr[x]->token != NULL)
			free(ptr[x]->token);
		x --;
	}
	if (ptr != NULL)
		free(ptr);
}

static size_t	sublen(const char *s, int i, int totlen)
{
	int	len;
	int	dquote;
	int	squote;

	len = 0;
	dquote = 0;
	squote = 0;
	while (i < totlen && s[i] == ' ')
		i ++;
	while (i < totlen && (s[i] != ' ' || dquote || squote))
	{
		len ++;
		if (s[i] == '"')
			dquote = !dquote;
		if (s[i] == '\'')
			squote = !squote;
		i ++;
	}
	return (len);
}

/*
 * Because of the 25 lines limit. I really don't like doing
 * this but you're forcing me, hope that makes you happy :D
*/
size_t	spltnmb(const char *s, int totlen)
{
	int	i[4];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	i[3] = 0;
	while (i[0] < totlen)
	{
		while (i[0] < totlen && s[i[0]] == ' ')
			i[0]++;
		if (i[0] < totlen && s[i[0]] != ' ')
		{
			while (i[0] < totlen && s[i[0]] != ' ')
			{
				if (s[i[0]] == '"' && !i[3])
					i[2] = !i[2];
				if (s[i[0]] == '\'' && !i[2])
					i[3] = !i[3];
				i[0]++;
			}
			if (!i[2] && !i[3])
				i[1]++;
		}
	}
	return (i[1]);
}

static char	**copy(t_quote *q, const char *s, char **ptr, size_t *i)
{
	while (*i < ft_strlen(s) && (s[*i] != ' ' || q->dquote || q->squote))
	{
		if (s[*i] == '"')
			q->dquote = !q->dquote;
		if (s[*i] == '\'')
			q->squote = !q->squote;
		ptr[q->x][q->y ++] = s[(*i)++];
	}
	ptr[q->x ++][q->y] = '\0';
	return (ptr);
}

char	**token_copy(char **ptr, const char *s, size_t i)
{
	t_quote	q;

	q.x = 0;
	q.dquote = 0;
	q.squote = 0;
	while (i < ft_strlen(s))
	{
		while (i < ft_strlen(s) && s[i] == ' ')
			i ++;
		if (i < ft_strlen(s) && s[i] != ' ')
		{
			ptr[q.x] = malloc(sublen(s, i, ft_strlen(s)) + 1);
			if (ptr[q.x] == NULL)
				return (free_tokens(ptr, q.x - 1), NULL);
			q.y = 0;
			ptr = copy(&q, s, ptr, &i);
		}
		else
			i ++;
	}
	ptr[q.x] = NULL;
	return (ptr);
}
