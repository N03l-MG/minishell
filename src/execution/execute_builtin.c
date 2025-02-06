/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:24:45 by jgraf             #+#    #+#             */
/*   Updated: 2025/02/06 21:26:09 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_echo(t_input *tokens, int start)
{
	int	i;
	int	j;
	int	has_n_flag;

	i = start + 1;
	has_n_flag = 0;
	if (tokens->token_count > 1)
	{
		while (i < tokens->token_count && tokens->tokens[i].token
			&& tokens->tokens[i].token[0] == '-'
			&& tokens->tokens[i].token[1] == 'n')
		{
			j = 2;
			while (tokens->tokens[i].token[j] == 'n')
				j ++;
			if (tokens->tokens[i].token[j] != '\0')
				break ;
			has_n_flag = 1;
			i ++;
		}
	}
	builtin_echo(tokens, has_n_flag, i);
}

void	handle_cd(t_input *tokens)
{
	int	i;

	i = 0;
	while (i < tokens->token_count
		&& ft_strcmp(tokens->tokens[i].token, "cd") != 0)
		i++;
	if (i + 1 >= tokens->token_count
		|| ft_strcmp(tokens->tokens[i + 1].token, "~") == 0)
		change_dir(tokens, my_getenv(tokens->env, "HOME"));
	else if (ft_strcmp(tokens->tokens[i + 1].token, "-") == 0)
		change_dir(tokens, my_getenv(tokens->env, "OLDPWD"));
	else
		change_dir(tokens, tokens->tokens[i + 1].token);
}

void	handle_export(t_input *tokens)
{
	int	i;
	int	cmd_pos;

	i = 0;
	while (i < tokens->token_count
		&& ft_strcmp(tokens->tokens[i].token, "export") != 0)
		i++;
	cmd_pos = i;
	if (cmd_pos + 1 < tokens->token_count
		&& tokens->tokens[cmd_pos + 1].type == STRING)
	{
		i = cmd_pos + 1;
		while (i < tokens->token_count && tokens->tokens[i].token != NULL)
		{
			if (tokens->tokens[i].type != STRING)
				break ;
			tokens->env = export_variable(tokens->tokens[i].token, tokens);
			i++;
		}
	}
	else
		print_sorted_env(tokens->env);
}

void	handle_unset(t_input *tokens)
{
	int	i;
	int	cmd_pos;

	i = 0;
	while (i < tokens->token_count
		&& ft_strcmp(tokens->tokens[i].token, "unset") != 0)
		i++;
	cmd_pos = i;
	if (cmd_pos + 1 < tokens->token_count
		&& tokens->tokens[cmd_pos + 1].type == STRING)
	{
		i = cmd_pos + 1;
		while (i < tokens->token_count && tokens->tokens[i].token != NULL)
		{
			if (tokens->tokens[i].type != STRING)
				break ;
			tokens->env = unset_variable(tokens->tokens[i].token, tokens);
			i++;
		}
	}
}

int	execute_builtin(t_input *tokens)
{
	char	*cmd;

	if (!tokens || !tokens->tokens || !tokens->tokens[0].token)
		return (1);
	cmd = tokens->tokens[0].token;
	if (ft_strcmp(cmd, "exit") == 0)
		clean_exit(tokens, NULL);
	if (ft_strcmp(cmd, "cd") == 0)
		handle_cd(tokens);
	else if (ft_strcmp(cmd, "echo") == 0)
		handle_echo(tokens, 0);
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
