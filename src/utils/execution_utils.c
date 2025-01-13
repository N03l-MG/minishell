/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:33:19 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/13 15:50:07 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_i(int *i, t_input *tokens)
{
	while (*i < tokens->token_count && ft_strcmp(tokens->tokens[*i].token, "|") != 0)
		(*i)++;
}

char	**parse_command(t_input tokens, int cmd_start, int cmd_end)
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
		cmd[j] = tokens.tokens[cmd_start + j].token;
		j++;
	}
	cmd[j] = NULL;
	return (cmd);
}

void	setup_pipe(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
		handle_error(PIPE_ERROR, NULL);
}

void	handle_child(t_data *data, int is_last, t_file *files, char **env)
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
	handle_redirections(files->infile, files->outfile);
	if (execve(data->full_path, data->cmd, env) == -1)
		handle_error(EXEC_ERROR, data->cmd[0]);
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
