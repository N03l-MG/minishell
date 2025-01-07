/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokentrim.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:42:54 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/06 14:42:57 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	find_start(char const *str, char const *set)
{
	int	i;
	int	j;

	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (set[j] != '\0')
		{
			if (str[i] == set[j])
				return (1);
			j ++;
		}
		i ++;
	}
	return (0);
}

static size_t	find_end(char const *str, char const *set)
{
	int	i;
	int	j;

	i = ft_strlen(str);
	while (i >= 0)
	{
		j = 0;
		while (set[j] != '\0')
		{
			if (str[i] == set[j])
				return (ft_strlen(str) - 1);
			j ++;
		}
		i --;
	}
	return (ft_strlen(str));
}

char	*ft_tokentrim(char const *s1, char const *set)
{
	char	*trimmed_s;
	size_t	start;
	size_t	end;
	size_t	l;

	start = find_start(s1, set);
	end = find_end(s1, set);
	l = end - start;
	trimmed_s = (char *)malloc(l + 1);
	if (!trimmed_s)
		return (NULL);
	ft_strlcpy(trimmed_s, &s1[start], l + 1);
	return (trimmed_s);
}
