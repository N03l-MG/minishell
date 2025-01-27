/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:05:06 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/27 11:05:07 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_child(t_data *data, char **env)
{
	if (is_builtin(data->cmd[0]))
	{
		execute_builtin_piped(data->cmd, env);
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (execve(data->full_path, data->cmd, env) == -1)
			handle_error(EXEC_ERROR, data->cmd[0], NULL);
	}
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
	handle_redir(files->infile, files->outfile, files->out_type);
	if (files->infile && strncmp(files->infile, "/tmp/.heredoc_", 13) == 0)
	{
		unlink(files->infile);
		free(files->infile);
	}
	execute_child(data, env);
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
	waitpid(pid, &data->status, 0);
}
