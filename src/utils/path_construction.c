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

char	*resolve_command_path(const char *command)
{
	char	*path_env;
	char	*path;
	char	*dir;
	char	full_path[1024];

	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	path_env = getenv("PATH");
	if (path_env == NULL)
		return (handle_error(ENV_NOT_FOUND, "PATH", NULL), NULL);
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
	return (free(path), NULL);
}
