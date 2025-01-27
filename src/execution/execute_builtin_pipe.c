/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:03:36 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/27 11:05:24 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_working_dir_piped(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		handle_error(EXEC_ERROR, "pwd", NULL);
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
	if (!cmd || !cmd[0])
		exit(handle_error(INVALID_INPUT, "empty command", NULL));
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
	else
		exit(handle_error(COMMAND_NOT_FOUND, cmd[0], NULL));
}
