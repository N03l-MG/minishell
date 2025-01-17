/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:21:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/17 13:04:00 by nmonzon          ###   ########.fr       */
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

static int	check_valid_pipes(t_input tokens)
{
	int		i;

	i = -1;
	while (++i < tokens.token_count)
	{
		if (ft_strcmp(tokens.tokens[i].token, "|") == 0)
		{
			if (i == 0 || i == tokens.token_count - 1)
				return (handle_error(INVALID_INPUT, tokens.tokens[i].token));
			if (i > 0 && ft_strcmp(tokens.tokens[i - 1].token, "|") == 0)
				return (handle_error(INVALID_INPUT, tokens.tokens[i].token));
		}
		if (ft_strcmp(tokens.tokens[i].token, "<") == 0
			|| ft_strcmp(tokens.tokens[i].token, ">") == 0)
		{
			if (i == tokens.token_count - 1)
				return (handle_error(INVALID_INPUT, tokens.tokens[i].token));
			if (ft_strcmp(tokens.tokens[i + 1].token, "|") == 0)
				return (handle_error(INVALID_INPUT, tokens.tokens[i].token));
		}
	}
	return (0);
}

static int	check_for_builtin(t_input tokens)
{
	int	i;

	i = -1;
	while (++i < tokens.token_count)
	{
		if (ft_strcmp(tokens.tokens[i].token, "exit") == 0)
			return (1);
		else if (ft_strcmp(tokens.tokens[i].token, "cd") == 0)
			return (1);
		else if (ft_strcmp(tokens.tokens[i].token, "echo") == 0)
			return (1);
		else if (ft_strcmp(tokens.tokens[i].token, "pwd") == 0)
			return (1);
		else if (ft_strcmp(tokens.tokens[i].token, "env") == 0)
			return (1);
		else if (ft_strcmp(tokens.tokens[i].token, "export") == 0)
			return (1);
		else if (ft_strcmp(tokens.tokens[i].token, "unset") == 0)
			return (1);
	}
	return (0);
}

static int	validate_command(t_input tokens, int start, int end)
{
	while (start < end)
	{
		if (tokens.tokens[start].type == REDIR_IN
			|| tokens.tokens[start].type == REDIR_OUT
			|| tokens.tokens[start].type == REDIR_APPEND)
		{
			start += 2;
			continue ;
		}
		if (tokens.tokens[start].type == STRING)
		{
			if (!is_command_valid(tokens.tokens[start].token))
				return (handle_error(COMMAND_NOT_FOUND,
						tokens.tokens[start].token));
			return (0);
		}
		start++;
	}
	return (1);
}

static bool	has_pipe(t_input tokens)
{
	int	i;

	i = 0;
	while (i < tokens.token_count)
	{
		if (tokens.tokens[i].type == PIPE)
			return (true);
		i++;
	}
	return (false);
}

int	validate_input(t_input *tokens)
{
	int	i;
	int	cmd_start;

	if (tokens->token_count == 0)
		return (1);
	if (check_valid_pipes(*tokens) > 0)
		return (1);
	if (check_for_builtin(*tokens) && !has_pipe(*tokens))
	{
		execute_builtin(tokens);
		return (1);
	}
	i = 0;
	while (i < tokens->token_count)
	{
		cmd_start = i;
		while (i < tokens->token_count && tokens->tokens[i].type != PIPE)
			i++;
		if (validate_command(*tokens, cmd_start, i) != 0)
			return (1);
		if (i < tokens->token_count && tokens->tokens[i].type == PIPE)
			i++;
	}
	return (0);
}
