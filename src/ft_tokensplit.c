/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokensplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 08:10:37 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/06 12:33:48 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_mem(char **ptr, int x)
{
	while (x > 0)
		free(ptr[--x]);
	free(ptr);
}

static size_t	calcsubstrlen(const char *s, int c, int i, int totlen)
{
	int	len;
	int	inquote;
	int	indquote;

	len = 0;
	inquote = 0;
	indquote = 0;
	while (i < totlen && (s[i] == c && !inquote && !indquote))
	{
		if (s[i] == '"' && !inquote)
			indquote = 1 - indquote;
		if (s[i] == '\'' && !indquote)
			inquote = 1 - inquote;
		i ++;
	}
	while (i < totlen && s[i] != c)
	{
		len ++;
		i ++;
	}
	return (len);
}

static size_t	spltnmb(const char *s, int c, int totlen)
{
	int	i;
	int	nmb;
	int	inquote;
	int	indquote;

	i = 0;
	nmb = 0;
	inquote = 0;
	indquote = 0;
	while (i < totlen)
	{
		while (i < totlen && (s[i] == c && !inquote && !indquote))
		{
			if (s[i] == '"' && !inquote)
				indquote = 1 - indquote;
			if (s[i] == '\'' && !indquote)
				inquote = 1 - inquote;
			i ++;
		}
		if (i < totlen && s[i] != c)
		{
			nmb ++;
			while (i < totlen && s[i] != c)
				i ++;
		}
	}
	return (nmb);
}

static char	**copy(char **ptr, const char *s, char c, size_t i)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (i < ft_strlen(s))
	{
		if (s[i] != c)
		{
			ptr[x] = malloc(calcsubstrlen(s, c, i, ft_strlen(s)) + 1);
			if (ptr[x] == NULL)
			{
				free_mem(ptr, x);
				return (NULL);
			}
			y = 0;
			while (s[i] != c && i < ft_strlen(s))
				ptr[x][y++] = s[i++];
			ptr[x++][y] = 0;
		}
		i ++;
	}
	ptr[x] = NULL;
	return (ptr);
}

char	**ft_tokensplit(const	char *s, char c)
{
	char	**ptr;
	int		i;
	int		x;
	int		totlen;

	if (s == NULL)
		return (NULL);
	i = 0;
	x = 0;
	totlen = ft_strlen(s);
	if (totlen == 0)
	{
		ptr = malloc(sizeof(char *));
		if (ptr == NULL)
			return (NULL);
		ptr[0] = 0;
		return (ptr);
	}
	ptr = malloc(sizeof(char *) * (spltnmb(s, c, totlen) + 1));
	if (ptr == NULL)
		return (NULL);
	while (s[i] == c)
		i ++;
	return (copy(ptr, s, c, i));
}
