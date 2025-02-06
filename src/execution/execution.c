/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:03:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/06 20:02:40 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_command(t_data *data)
{
	if (data->cmd)
	{
		free_cmd_array(data->cmd);
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

static void	handle_lone_redir(t_file files, t_process_args *args)
{
	int	fd;

	if (files.infile)
	{
		fd = open(files.infile, O_RDONLY);
		if (fd == -1)
		{
			handle_error(INVALID_FILE, files.infile, args->tokens);
			return ;
		}
		close(fd);
	}
	if (files.outfile)
	{
		fd = open(files.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd != -1)
			close(fd);
	}
	if (files.infile)
		free(files.infile);
	if (files.outfile)
		free(files.outfile);
}

static void	process_command(t_process_args *args)
{
	t_file	files;

	get_redir(*args->tokens, &args->cmd_start, &args->cmd_end, &files);
	args->data->cmd = parse_cmd(*args->tokens, args->cmd_start, args->cmd_end);
	if (!args->data->cmd && (files.infile || files.outfile))
	{
		handle_lone_redir(files, args);
		return ;
	}
	if (args->data->cmd && args->data->cmd[0])
	{
		args->data->full_path = resolve_command_path(args->tokens->env,
				args->data->cmd[0]);
		if (!args->data->full_path && !is_builtin(args->data->cmd[0]))
			handle_error(COMMAND_NOT_FOUND, args->data->cmd[0], args->tokens);
		else if (access(args->data->full_path, X_OK) != 0
			&& !is_builtin(args->data->cmd[0]))
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
	if (tokens->tokens[0].token[0] == '\0')
		return ;
	data.prev_fd = -1;
	data.tokens = tokens;
	while (i < tokens->token_count)
	{
		cmd_start = i;
		while (i < tokens->token_count && tokens->tokens[i].type != PIPE)
			i++;
		data.cmd_start = cmd_start;
		process_command(&(t_process_args){&data, tokens, cmd_start, i});
		if (i < tokens->token_count && tokens->tokens[i].type == PIPE)
			i++;
	}
}
