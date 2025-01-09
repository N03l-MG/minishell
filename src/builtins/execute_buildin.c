/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_buildin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:24:45 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/09 14:24:48 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_buildin(t_token tokens, char **env)
{
	if (ft_strcmp(tokens.tokens[0], "exit") == 0)
		return (-1);
	if (ft_strcmp(tokens.tokens[0], "cd") == 0)
	{
		if (tokens.token_count >= 2)
			change_directory(tokens.tokens[1]);
		else if (tokens.token_count == 1)
			change_directory(getenv("HOME"));
	}
	else if (ft_strcmp(tokens.tokens[0], "echo") == 0)
	{
		if (ft_strcmp(tokens.tokens[1], "-n") == 0)
			buildin_echo(tokens, 1, 0);
		else
			buildin_echo(tokens, 0, 0);
	}
	else if (ft_strcmp(tokens.tokens[0], "pwd") == 0)
		print_working_dir();
	else if (ft_strcmp(tokens.tokens[0], "env") == 0)
		print_envs(env);
	else if (ft_strcmp(tokens.tokens[0], "export") == 0)
		export_variable(env, tokens.tokens[1]);
	return (1);
}
