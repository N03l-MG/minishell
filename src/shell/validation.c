/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:21:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/09 15:49:45 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_command_valid(const char *command)
{
	char	*path_env;
	char	*path;
	char	*dir;
	char	full_path[1024];

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
		if (ft_strcmp(tokens.tokens[i], "<") == 0
			|| ft_strcmp(tokens.tokens[i], ">") == 0)
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
	return (0);
}
