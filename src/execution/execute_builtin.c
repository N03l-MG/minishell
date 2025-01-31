/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:24:45 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/28 12:31:52 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_echo(t_input *tokens)
{
	int	has_n_flag;

	has_n_flag = 0;
	if (tokens->token_count > 1 && tokens->tokens[1].token)
	{
		if (ft_strcmp(tokens->tokens[1].token, "-n") == 0)
			has_n_flag = 1;
	}
	builtin_echo(tokens, has_n_flag);
}

static void	handle_cd(t_input *tokens)
{
	if (tokens->token_count >= 2)
		change_dir(tokens, tokens->tokens[1].token);
	else
		change_dir(tokens, getenv("HOME"));
}

static void	handle_export(t_input *tokens)
{
	if (tokens->token_count > 1)
		tokens->env = export_variable(tokens->tokens[1].token, *tokens);
	else
		print_sorted_env(tokens->env);
}

static void	handle_unset(t_input *tokens)
{
	if (tokens->token_count > 1)
		tokens->env = unset_variable(tokens->tokens[1].token, *tokens);
}

int	execute_builtin(t_input *tokens)
{
	char	*cmd;

	if (!tokens || !tokens->tokens || !tokens->tokens[0].token)
		return (1);
	cmd = tokens->tokens[0].token;
	if (ft_strcmp(cmd, "exit") == 0)
		clean_exit(tokens);
	if (ft_strcmp(cmd, "cd") == 0)
		handle_cd(tokens);
	else if (ft_strcmp(cmd, "echo") == 0)
		handle_echo(tokens);
	else if (ft_strcmp(cmd, "pwd") == 0)
		print_working_dir(tokens);
	else if (ft_strcmp(cmd, "env") == 0)
		print_envs(tokens);
	else if (ft_strcmp(cmd, "export") == 0)
		handle_export(tokens);
	else if (ft_strcmp(cmd, "unset") == 0)
		handle_unset(tokens);
	return (1);
}
