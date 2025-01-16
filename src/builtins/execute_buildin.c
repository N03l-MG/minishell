/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_buildin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:24:45 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/16 13:37:05 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_input tokens, char **env)
{
	if (ft_strcmp(tokens.tokens[0].token, "exit") == 0)
		exit(EXIT_SUCCESS);
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

// Example of modified builtin function
void	print_working_dir_piped(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		handle_error(EXEC_ERROR, "pwd");
}

void	builtin_echo_piped(char **cmd, int no_nl)
{
	int	i;

	if (no_nl)
		i = 2;
	else
		i = 1;
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (!no_nl)
		printf("\n");
}

void	print_envs_piped(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

// Add other builtins that can output to pipes
void	execute_builtin_piped(char **cmd, char **env)
{
	if (ft_strcmp(cmd[0], "echo") == 0)
	{
		if (cmd[1] && ft_strcmp(cmd[1], "-n") == 0)
			builtin_echo_piped(cmd, 1);
		else
			builtin_echo_piped(cmd, 0);
	}
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		print_working_dir_piped();
	else if (ft_strcmp(cmd[0], "env") == 0)
		print_envs_piped(env);
}
