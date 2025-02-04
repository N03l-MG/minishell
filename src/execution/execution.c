/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:03:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/04 15:18:53 by nmonzon          ###   ########.fr       */
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

static void	execute_command(t_process_args args, t_file *files)
{
	pid_t	pid;

	if (args.cmd_end < args.tokens->token_count
		&& args.tokens->tokens[args.cmd_end].type == PIPE)
		setup_pipe(args.data->pipe_fds);
	pid = fork();
	if (pid == -1)
		handle_error(FORK_ERROR, NULL, args.tokens);
	else if (pid == 0)
		handle_child(args.data, (args.cmd_end == args.tokens->token_count),
			files, args.tokens->env);
	else
	{
		handle_parent(args.data, &args.data->prev_fd, pid,
			(args.cmd_end == args.tokens->token_count));
		if (args.cmd_end == args.tokens->token_count)
		{
			waitpid(pid, &args.data->status, 0);
			if (WIFSIGNALED(args.data->status))
				args.tokens->last_status = 128 + WTERMSIG(args.data->status);
			else if (WIFEXITED(args.data->status))
				args.tokens->last_status = WEXITSTATUS(args.data->status);
		}
	}
}

static void	process_command(t_process_args *args)
{
	t_file	files;

	get_redir(*args->tokens, &args->cmd_start, &args->cmd_end, &files);
	args->data->cmd = parse_cmd(*args->tokens, args->cmd_start, args->cmd_end);
	if (!args->data->cmd && (files.infile || files.outfile))
	{
		args->data->cmd = malloc(2 * sizeof(char *));
		if (!args->data->cmd)
			handle_fatal_error(MEMORY_ERROR, NULL, args->tokens);
		args->data->cmd[0] = ft_strdup("cat");
		args->data->cmd[1] = NULL;
	}
	if (args->data->cmd && args->data->cmd[0])
	{
		args->data->full_path = resolve_command_path(args->data->cmd[0]);
		if (!args->data->full_path)
			handle_error(COMMAND_NOT_FOUND, args->data->cmd[0], args->tokens);
		else if (access(args->data->full_path, X_OK) != 0)
			handle_error(PERMISSION_ERROR, args->data->cmd[0], args->tokens);
		else
			execute_command(*args, &files);
	}
	cleanup_command(args->data);
}

void	execute_input(t_input *tokens)
{
	t_data	data;
	int		i;
	int		cmd_start;

	i = 0;
	data.prev_fd = -1;
	data.tokens = tokens;
	while (i < tokens->token_count)
	{
		cmd_start = i;
		while (i < tokens->token_count && tokens->tokens[i].type != PIPE)
			i++;
		process_command(&(t_process_args){&data, tokens, cmd_start, i});
		if (i < tokens->token_count && tokens->tokens[i].type == PIPE)
			i++;
	}
}
