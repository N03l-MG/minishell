/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:03:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/07 16:21:30 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

static char	**parse_command(t_token tokens, int cmd_start, int cmd_end)
{
	char	**cmd;
	int		j;
	int		size;

	size = cmd_end - cmd_start + 1;
	cmd = malloc(size * sizeof(char *));
	if (!cmd)
		exit(EXIT_FAILURE);
	j = 0;
	while (j < cmd_end - cmd_start)
	{
		cmd[j] = tokens.tokens[cmd_start + j];
		j++;
	}
	cmd[j] = NULL;
	return (cmd);
}

static void	setup_pipe(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
		exit(EXIT_FAILURE);
}

static void	handle_child(t_data *data, int is_last)
{
	if (data->prev_fd != -1)
	{
		dup2(data->prev_fd, STDIN_FILENO);
		close(data->prev_fd);
	}
	if (!is_last)
	{
		close(data->pipe_fds[0]);
		dup2(data->pipe_fds[1], STDOUT_FILENO);
		close(data->pipe_fds[1]);
	}
	if (execve(data->full_path, data->cmd, environ) == -1)
		exit(EXIT_FAILURE);
}

static void	handle_parent(t_data *data, int *prev_fd, pid_t pid, int is_last)
{
	if (!is_last)
		close(data->pipe_fds[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (!is_last)
		*prev_fd = data->pipe_fds[0];
	else
		*prev_fd = -1;
	waitpid(pid, NULL, 0);
}

void	execute_input(t_token tokens)
{
	t_data	data;
	int		i;
	int		cmd_start;
	pid_t	pid;

	data.prev_fd = -1;
	i = 0;
	while (i < tokens.token_count)
	{
		cmd_start = i;
		while (i < tokens.token_count && ft_strcmp(tokens.tokens[i], "|") != 0)
			i++;
		data.cmd = parse_command(tokens, cmd_start, i);
		data.full_path = resolve_command_path(data.cmd[0]);
		if (!(i == tokens.token_count))
			setup_pipe(data.pipe_fds);
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
			handle_child(&data, (i == tokens.token_count));
		else
			handle_parent(&data, &data.prev_fd, pid, (i == tokens.token_count));
		free(data.cmd);
		free(data.full_path);
		i++;
	}
}
