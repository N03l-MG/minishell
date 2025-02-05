/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_construction.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:04:38 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/05 16:24:04 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*try_raw_path(const char *command)
{
	struct stat	path_stat;

	if (stat(command, &path_stat) != 0)
		return (NULL);
	if (!S_ISREG(path_stat.st_mode))
	{
		errno = EISDIR;
		return (NULL);
	}
	if (access(command, F_OK) != 0)
		return (NULL);
	return (ft_strdup(command));
}

char	*resolve_command_path(const char *command)
{
	char		*path_env;
	char		*path;
	char		*dir;
	char		full_path[1024];

	if (ft_strchr(command, '/'))
		return (try_raw_path(command));
	path_env = getenv("PATH");
	if (path_env == NULL)
		return (handle_error(ENV_NOT_FOUND, "PATH", NULL), NULL);
	path = ft_strdup(path_env);
	dir = ft_strtok(path, ":");
	while (dir != NULL)
	{
		build_path(full_path, dir, command);
		if (access(full_path, F_OK) == 0)
		{
			free(path);
			return (ft_strdup(full_path));
		}
		dir = ft_strtok(NULL, ":");
	}
	return (free(path), NULL);
}
