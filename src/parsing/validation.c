/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:21:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/05 12:33:31 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_valid_redir(t_input *tok, int i)
{
	if (i == tok->token_count - 1)
		return (handle_error(SYNTAX_ERROR, "newline", tok));
	if (!tok->tokens[i + 1].token)
		return (handle_error(SYNTAX_ERROR, "newline", tok));
	if (tok->tokens[i + 1].type == PIPE
		|| tok->tokens[i + 1].type == REDIR_IN
		|| tok->tokens[i + 1].type == REDIR_OUT
		|| tok->tokens[i + 1].type == APPEND
		|| tok->tokens[i + 1].type == HEREDOC)
		return (handle_error(SYNTAX_ERROR, tok->tokens[i + 1].token, tok));
	return (0);
}

static int	check_valid_pipes(t_input *t)
{
	int	i;

	i = -1;
	if (t->tokens[0].type == PIPE)
		return (handle_error(SYNTAX_ERROR, "|", t));
	while (++i < t->token_count)
	{
		if (!t->tokens[i].token)
			continue ;
		if (t->tokens[i].type == PIPE)
		{
			if (i == t->token_count - 1)
				return (handle_error(SYNTAX_ERROR, "|", t));
			if ((i > 0 && t->tokens[i - 1].type == PIPE)
				|| (t->tokens[i + 1].token && t->tokens[i + 1].type == PIPE))
				return (handle_error(SYNTAX_ERROR, "||", t));
		}
		else if (t->tokens[i].type == REDIR_IN || t->tokens[i].type == REDIR_OUT
			|| t->tokens[i].type == APPEND || t->tokens[i].type == HEREDOC)
		{
			if (check_valid_redir(t, i) != 0)
				return (1);
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
			|| tokens.tokens[i].type == APPEND)
			return (true);
		i++;
	}
	return (false);
}

int	validate_input(t_input *tokens)
{
	if (!tokens || !tokens->tokens || tokens->token_count == 0
		|| tokens->quote_error == true
		|| check_valid_pipes(tokens) > 0)
		return (1);
	if (check_for_builtin(*tokens) && !has_pipe(*tokens))
	{
		execute_builtin(tokens);
		return (1);
	}
	return (0);
}
