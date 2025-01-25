/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:53:58 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/21 14:43:42 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*saved_str;
	char		*token;

	if (str != NULL)
		saved_str = str;
	if (saved_str == NULL || *saved_str == '\0')
		return (NULL);
	while (*saved_str && ft_strchr(delim, *saved_str))
		saved_str++;
	if (*saved_str == '\0')
		return (NULL);
	token = saved_str;
	while (*saved_str && !ft_strchr(delim, *saved_str))
		saved_str++;
	if (*saved_str)
	{
		*saved_str = '\0';
		saved_str++;
	}
	return (token);
}

bool	is_builtin(const char *cmd)
{
	return (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*result;
	size_t	len;

	len = ft_strlen(s);
	if (len > n)
		len = n;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s, len + 1);
	return (result);
}
