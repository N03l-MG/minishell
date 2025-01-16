/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:53:58 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/16 13:27:13 by nmonzon          ###   ########.fr       */
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

void	build_path(char *full_path, const char *path, const char *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (path[i] != '\0')
	{
		full_path[i] = path[i];
		i++;
	}
	full_path[i] = '/';
	i++;
	while (cmd[j] != '\0')
	{
		full_path[i] = cmd[j];
		i++;
		j++;
	}
	full_path[i] = '\0';
}

char	*resolve_command_path(const char *command)
{
	char	*path_env;
	char	*path;
	char	*dir;
	char	full_path[1024];

	path_env = getenv("PATH");
	if (path_env == NULL)
	{
		handle_error(ENV_NOT_FOUND, NULL);
		return (NULL);
	}
	path = ft_strdup(path_env);
	dir = ft_strtok(path, ":");
	while (dir != NULL)
	{
		build_path(full_path, dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(path);
			return (ft_strdup(full_path));
		}
		dir = ft_strtok(NULL, ":");
	}
	free(path);
	return (NULL);
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
