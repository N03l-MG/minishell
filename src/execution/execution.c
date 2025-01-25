/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:03:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/24 11:16:30 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_command(t_data *data)
{
	if (data->cmd)
	{
		free(data->cmd);
		data->cmd = NULL;
	}
	if (data->full_path)
	{
		free(data->full_path);
		data->full_path = NULL;
	}
}

static void	execute_command(t_data *data, t_file *files, t_input tokens,
						int i, int *last_status)
{
	pid_t	pid;

	if (i < tokens.token_count && tokens.tokens[i].type == PIPE)
		setup_pipe(data->pipe_fds);
	pid = fork();
	if (pid == -1)
		handle_error(FORK_ERROR, NULL, &tokens);
	else if (pid == 0)
		handle_child(data, (i == tokens.token_count), files, tokens.env);
	else
	{
		handle_parent(data, &data->prev_fd, pid, (i == tokens.token_count));
		if (i == tokens.token_count && WEXITSTATUS(data->status))
			*last_status = WEXITSTATUS(data->status);
	}
}

static void	process_command(t_data *data, t_input tokens, int cmd_start, 
						int i, int *last_status)
{
	t_file	files;

	get_redir(tokens, &cmd_start, &i, &files);
	data->cmd = parse_command(tokens, cmd_start, i);
	if (!data->cmd)
	{
		if (files.infile || files.outfile)
		{
			data->cmd = malloc(2 * sizeof(char *));
			data->cmd[0] = ft_strdup("cat");
			data->cmd[1] = NULL;
			data->full_path = resolve_command_path(data->cmd[0]);
			execute_command(data, &files, tokens, i, last_status);
		}
	}
	else
	{
		data->full_path = resolve_command_path(data->cmd[0]);
		if (!data->full_path)
		{
			handle_error(COMMAND_NOT_FOUND, data->cmd[0], &tokens);
			if (i == tokens.token_count)
				*last_status = 127;
		}
		else
			execute_command(data, &files, tokens, i, last_status);
	}
	cleanup_command(data);
}

void	execute_input(t_input tokens)
{
	t_data	data;
	int		i;
	int		cmd_start;
	int		last_status;

	i = 0;
	data.prev_fd = -1;
	last_status = 0;
	while (i < tokens.token_count)
	{
		cmd_start = i;
		while (i < tokens.token_count && tokens.tokens[i].type != PIPE)
			i++;
		process_command(&data, tokens, cmd_start, i, &last_status);
		if (i < tokens.token_count && tokens.tokens[i].type == PIPE)
			i++;
	}
	tokens.env = export_variable_sep("LASTSTATUS", ft_itoa(last_status),
			tokens);
}
