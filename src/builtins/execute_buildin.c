/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_buildin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:24:45 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/13 15:21:55 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_buildin(t_input tokens, char **env)
{
	if (ft_strcmp(tokens.tokens[0].token, "exit") == 0)
		return (-1);
	if (ft_strcmp(tokens.tokens[0].token, "cd") == 0)
	{
		if (tokens.token_count >= 2)
			change_directory(tokens.tokens[1].token);
		else if (tokens.token_count == 1)
			change_directory(getenv("HOME"));
	}
	else if (ft_strcmp(tokens.tokens[0].token, "echo") == 0)
	{
		if (ft_strcmp(tokens.tokens[1].token, "-n") == 0)
			buildin_echo(tokens, 1, 0);
		else
			buildin_echo(tokens, 0, 0);
	}
	else if (ft_strcmp(tokens.tokens[0].token, "pwd") == 0)
		print_working_dir();
	else if (ft_strcmp(tokens.tokens[0].token, "env") == 0)
		print_envs(env);
	else if (ft_strcmp(tokens.tokens[0].token, "export") == 0)
		export_variable(env, tokens.tokens[1].token);
	return (1);
}
