/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kixik   <github.com/kixikCodes>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:05:06 by kixik             #+#    #+#             */
/*   Updated: 2025/02/06 21:49:04 by kixik            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_array(char **cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

static void	execute_child(t_data *data, char **env)
{
	if (is_builtin(data->cmd[0]))
	{
		if (ft_strcmp(data->cmd[0], "echo") == 0)
			handle_echo(data->tokens, data->cmd_start);
		else if (ft_strcmp(data->cmd[0], "pwd") == 0)
			print_working_dir(data->tokens);
		else if (ft_strcmp(data->cmd[0], "env") == 0)
			print_envs(data->tokens);
		else if (ft_strcmp(data->cmd[0], "cd") == 0)
			handle_cd(data->tokens);
		else if (ft_strcmp(data->cmd[0], "export") == 0)
			handle_export(data->tokens);
		else if (ft_strcmp(data->cmd[0], "unset") == 0)
			handle_unset(data->tokens);
		free_cmd_array(data->cmd);
		if (data->full_path)
			free(data->full_path);
		exit(data->tokens->last_status);
	}
	else if (execve(data->full_path, data->cmd, env) == -1)
		handle_error(EXEC_ERROR, data->cmd[0], data->tokens);
	clean_exit(data->tokens, data);
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
	handle_redir(files->infile, files->outfile, files->out_type, data->tokens);
	if (files->infile && strncmp(files->infile, "/tmp/.heredoc_", 13) == 0)
	{
		unlink(files->infile);
		free(files->infile);
	}
	execute_child(data, env);
}

void	handle_parent(t_data *data, int *prev_fd, int is_last)
{
	if (!is_last)
		close(data->pipe_fds[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (!is_last)
		*prev_fd = data->pipe_fds[0];
	else
		*prev_fd = -1;
}
