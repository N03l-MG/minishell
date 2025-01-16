/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:24:45 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/16 17:03:15 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_exe(char *path, char **op, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	status = INT_MIN;
	if (pid == -1)
	{
		ft_fprintf(2, "Failed to create child process.\n");
		return (0);
	}
	if (pid == 0)
	{
		if (execve(path, op, env) == -1)
		{
			ft_fprintf(2, "Failed to execute %s\n", path);
			exit(EXIT_SUCCESS);
		}
	}
	else
		wait(&status);
	printf("%i\n", status);		//Placehilder. Add struct for this
	return (1);
}

int	execute_builtin(t_input tokens)
{
	if (ft_strcmp(tokens.tokens[0].token, "exit") == 0)
		exit(EXIT_SUCCESS);
	if (ft_strcmp(tokens.tokens[0].token, "cd") == 0)
	{
		if (tokens.token_count >= 2)
			change_dir(&tokens, tokens.tokens[1].token);
		else if (tokens.token_count == 1)
			change_dir(&tokens, getenv("HOME"));
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
		print_envs(tokens.env);
	else if (ft_strcmp(tokens.tokens[0].token, "export") == 0)
		export_variable(tokens.tokens[1].token, tokens);
	else if (ft_strncmp(tokens.tokens[0].token, "./", 2) == 0)
		execute_exe(tokens.tokens[0].token, &tokens.tokens->token, tokens.env);
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
