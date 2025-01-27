/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:21:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/27 13:22:04 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_valid_pipes(t_input tok)
{
	int	i;

	i = 0;
	while (++i < tok.token_count)
	{
		if (!tok.tokens[i].token)
			continue ;
		if (ft_strcmp(tok.tokens[i].token, "|") == 0)
		{
			if (i == 0 || i == tok.token_count - 1
				|| (i > 0 && ft_strcmp(tok.tokens[i - 1].token, "|") == 0))
				return (handle_error(INVALID_INPUT, tok.tokens[i].token, &tok));
		}
		if (ft_strcmp(tok.tokens[i].token, "<") == 0
			|| ft_strcmp(tok.tokens[i].token, ">") == 0)
		{
			if (i == tok.token_count - 1
				|| (tok.tokens[i + 1].token
					&& ft_strcmp(tok.tokens[i + 1].token, "|") == 0))
				return (handle_error(INVALID_INPUT, tok.tokens[i].token, &tok));
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
		if (!tokens.tokens[i].token)
			continue ;
		if (ft_strcmp(tokens.tokens[i].token, "exit") == 0
			|| ft_strcmp(tokens.tokens[i].token, "cd") == 0
			|| ft_strcmp(tokens.tokens[i].token, "echo") == 0
			|| ft_strcmp(tokens.tokens[i].token, "pwd") == 0
			|| ft_strcmp(tokens.tokens[i].token, "env") == 0
			|| ft_strcmp(tokens.tokens[i].token, "export") == 0
			|| ft_strcmp(tokens.tokens[i].token, "unset") == 0)
			return (1);
	}
	return (0);
}

static bool	has_pipe(t_input tokens)
{
	int	i;

	i = 0;
	while (i < tokens.token_count)
	{
		if (tokens.tokens[i].type == PIPE
			|| tokens.tokens[i].type == REDIR_IN
			|| tokens.tokens[i].type == REDIR_OUT
			|| tokens.tokens[i].type == REDIR_APPEND)
			return (true);
		i++;
	}
	return (false);
}

int	validate_input(t_input *tokens)
{
	if (!tokens || !tokens->tokens || tokens->token_count == 0
		|| tokens->quote_error == true
		|| check_valid_pipes(*tokens) > 0)
		return (1);
	if (check_for_builtin(*tokens) && !has_pipe(*tokens))
	{
		execute_builtin(tokens);
		return (1);
	}
	return (0);
}
