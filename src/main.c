/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:44:32 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/06 17:16:12 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*resolve_command_path(const char *command);
static void	execute_input(t_token tokens);
extern char	**environ;

int	main(void)
{
	char	*prompt;
	t_token	tokens;

	signal(SIGINT, SIG_IGN);
	while (true)
	{
		prompt = readline("minishell> ");
		if (prompt != NULL)
		{
			if (*prompt == '\0')
			{
				free(prompt);
				continue ;
			}
			if (ft_strcmp(prompt, "exit") == 0)
			{
				free(prompt);
				break ;
			}
			tokens = create_tokens(prompt);
			add_history(prompt);
			if (validate_input(tokens) == 0)
				execute_input(tokens);
			free(prompt);
			free(tokens.tokens);
		}
		else
			ft_fprintf(2, "Error: Could not read input.\n");
	}
	return (0);
}

static char	*resolve_command_path(const char *command)
{
	char	*path_env;
	char	*path;
	char	*dir;
	char	full_path[1024];

	if (command == NULL || *command == '\0')
		return (NULL);
	path_env = getenv("PATH");
	if (path_env == NULL)
	{
		handle_error(ENV_NOT_FOUND, NULL);
		return (NULL);
	}
	path = ft_strdup(path_env);
	dir = ft_strtok(path, ":");
	while (dir != NULL)
	{
		build_path(full_path, dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(path);
			return (ft_strdup(full_path));
		}
		dir = ft_strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}

static void	execute_input(t_token tokens) // Massive norminette violation
{
	int		i;
	int		pipe_fds[2];
	int		prev_fd;
	pid_t	pid;
	char	*full_path;
	char	**cmd;
	int		cmd_start;

	i = 0;
	prev_fd = -1;
	while (i < tokens.token_count)
	{
		cmd = NULL;
		cmd_start = i;
		while (i < tokens.token_count && ft_strcmp(tokens.tokens[i], "|") != 0)
			i++;
		cmd = malloc((i - cmd_start + 1) * sizeof(char *));
		for (int j = 0; j < i - cmd_start; j++)
			cmd[j] = tokens.tokens[cmd_start + j];
		cmd[i - cmd_start] = NULL;
		full_path = resolve_command_path(cmd[0]);
		if (i < tokens.token_count && ft_strcmp(tokens.tokens[i], "|") == 0)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (i < tokens.token_count && ft_strcmp(tokens.tokens[i], "|") == 0)
			{
				close(pipe_fds[0]);
				dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[1]);
			}
			if (execve(full_path, cmd, environ) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (i < tokens.token_count && ft_strcmp(tokens.tokens[i], "|") == 0)
				close(pipe_fds[1]);
			if (prev_fd != -1)
				close(prev_fd);
			if (i < tokens.token_count && ft_strcmp(tokens.tokens[i], "|") == 0)
				prev_fd = pipe_fds[0];
			else
				prev_fd = -1;
			waitpid(pid, NULL, 0);
		}
		i++;
	}
}
