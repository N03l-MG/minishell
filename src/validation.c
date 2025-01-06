/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:21:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/06 16:08:53 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	build_path(char *full_path, const char *path, const char *cmd)
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

static bool	is_command_valid(const char *command)
{
	char	*path_env;
	char	*path;
	char	*dir;
	char	full_path[1024];

	if (command == NULL || *command == '\0')
		return (false);
	path_env = getenv("PATH");
	if (path_env == NULL)
	{
		handle_error(ENV_NOT_FOUND, NULL);
		return (false);
	}
	path = ft_strdup(path_env);
	dir = ft_strtok(path, ":");
	while (dir != NULL)
	{
		build_path(full_path, dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(path);
			return (true);
		}
		dir = ft_strtok(NULL, ":");
	}
	free(path);
	return (false);
}

static int	check_valid_chars(t_token tokens)
{
	int		i;
	char	*c;

	i = -1;
	while (++i < tokens.token_count)
	{
		c = tokens.tokens[i];
		while (*c != '\0')
		{
			if (!ft_isalnum(*c) && !ft_strchr("-_/.|<>", *c))
				return (handle_error(INVALID_INPUT, tokens.tokens[i]));
			c++;
		}
	}
	return (0);
}

static int	check_valid_pipes(t_token tokens)
{
	int		i;

	i = -1;
	while (++i < tokens.token_count)
	{
		if (ft_strcmp(tokens.tokens[i], "|") == 0)
		{
			if (i == 0 || i == tokens.token_count - 1)
				return (handle_error(INVALID_INPUT, tokens.tokens[i]));
			if (i > 0 && ft_strcmp(tokens.tokens[i - 1], "|") == 0)
				return (handle_error(INVALID_INPUT, tokens.tokens[i]));
		}
		if (ft_strcmp(tokens.tokens[i], "<") == 0 || ft_strcmp(tokens.tokens[i], ">") == 0)
		{
			if (i == tokens.token_count - 1)
				return (handle_error(INVALID_INPUT, tokens.tokens[i]));
			if (ft_strcmp(tokens.tokens[i + 1], "|") == 0)
				return (handle_error(INVALID_INPUT, tokens.tokens[i]));
		}
	}
	return (0);
}

int	validate_input(t_token tokens)
{
	int	i;

	if (tokens.token_count == 0)
		return (1);
	if (check_valid_chars(tokens) > 0)
		return (1);
	if (check_valid_pipes(tokens) > 0)
		return (1);
	i = -1;
	while (++i < tokens.token_count)
	{
		if (i == 0 || (ft_strcmp(tokens.tokens[i - 1], "|") == 0))
		{
			if (!is_command_valid(tokens.tokens[i]))
				return (handle_error(COMMAND_NOT_FOUND, tokens.tokens[i]));
		}
	}
	//printf("Input validation passed.\n");
	return (0);
}
