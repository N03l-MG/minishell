/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:33:19 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/09 13:43:14 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

void	set_i(int *i, t_token *tokens)
{
	while (*i < tokens->token_count && ft_strcmp(tokens->tokens[*i], "|") != 0)
		(*i)++;
}

char	**parse_command(t_token tokens, int cmd_start, int cmd_end)
{
	char	**cmd;
	int		j;
	int		size;

	size = cmd_end - cmd_start + 1;
	cmd = malloc(size * sizeof(char *));
	if (!cmd)
		handle_mem_error(&tokens);
	j = 0;
	while (j < cmd_end - cmd_start)
	{
		cmd[j] = tokens.tokens[cmd_start + j];
		j++;
	}
	cmd[j] = NULL;
	return (cmd);
}

void	setup_pipe(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
		handle_error(PIPE, NULL);
}

void	handle_child(t_data *data, int is_last)
{
	if (data->prev_fd != -1)
	{
		dup2(data->prev_fd, 0);
		close(data->prev_fd);
	}
	if (!is_last)
	{
		close(data->pipe_fds[0]);
		dup2(data->pipe_fds[1], 1);
		close(data->pipe_fds[1]);
	}
	if (execve(data->full_path, data->cmd, environ) == -1)
		handle_error(EXEC, data->cmd[0]);
}

void	handle_parent(t_data *data, int *prev_fd, pid_t pid, int is_last)
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
