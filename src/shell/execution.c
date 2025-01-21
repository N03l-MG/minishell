/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:03:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/21 15:03:51 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_redir(t_input tokens, int *cmd_start, int *cmd_end, t_file *files)
{
	int	i;

	i = *cmd_start;
	files->infile = NULL;
	files->outfile = NULL;
	files->out_type = REDIR_OUT;
	while (i < *cmd_end)
	{
		if (tokens.tokens[i].type == REDIR_IN
			|| tokens.tokens[i].type == REDIR_OUT
			|| tokens.tokens[i].type == REDIR_APPEND)
		{
			if (i + 1 < *cmd_end)
			{
				if (tokens.tokens[i].type == REDIR_OUT
					|| tokens.tokens[i].type == REDIR_APPEND)
				{
					files->outfile = tokens.tokens[i + 1].token;
					files->out_type = tokens.tokens[i].type;
				}
				else if (tokens.tokens[i].type == REDIR_IN)
					files->infile = tokens.tokens[i + 1].token;
				tokens.tokens[i].type = END;
				tokens.tokens[i + 1].type = END;
				i += 2;
			}
			else
			{
				handle_error(INVALID_INPUT, tokens.tokens[i].token, &tokens);
				return ;
			}
		}
		else
			i++;
	}
}

void	handle_redir(char *input_file, char *output_file, t_ttype out_type)
{
	int	fd;
	int	flags;

	if (input_file)
	{
		fd = open(input_file, O_RDONLY);
		if (fd == -1)
			handle_error(INVALID_FILE, input_file, NULL);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (output_file)
	{
		flags = O_CREAT | O_WRONLY;
		if (out_type == REDIR_APPEND)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(output_file, flags, 0644);
		if (fd == -1)
			handle_error(INVALID_FILE, output_file, NULL);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	execute_input(t_input tokens)
{
	t_data	data;
	int		i;
	int		cmd_start;
	pid_t	pid;
	t_file	files;
	int		status;
	int		last_status;

	i = 0;
	data.prev_fd = -1;
	last_status = 0;
	while (i < tokens.token_count)
	{
		cmd_start = i;
		while (i < tokens.token_count && tokens.tokens[i].type != PIPE)
			i++;
		files.infile = NULL;
		files.outfile = NULL;
		files.out_type = REDIR_OUT;
		get_redir(tokens, &cmd_start, &i, &files);
		data.cmd = parse_command(tokens, cmd_start, i);
		data.full_path = resolve_command_path(data.cmd[0]);
		if (!data.full_path)
		{
			handle_error(COMMAND_NOT_FOUND, data.cmd[0], &tokens);
			if (i == tokens.token_count)
				last_status = 127;
		}
		else
		{
			if (i < tokens.token_count && tokens.tokens[i].type == PIPE)
				setup_pipe(data.pipe_fds);
			pid = fork();
			if (pid == -1)
				handle_error(FORK_ERROR, NULL, &tokens);
			else if (pid == 0)
				handle_child(&data, (i == tokens.token_count), &files, tokens.env);
			else
			{
				handle_parent(&data, &data.prev_fd, pid, (i == tokens.token_count), &status);
				if (i == tokens.token_count && WEXITSTATUS(status))
					last_status = WEXITSTATUS(status);
			}
		}
		if (data.cmd)
		{
			free(data.cmd);
			data.cmd = NULL;
		}
		if (data.full_path)
		{
			free(data.full_path);
			data.full_path = NULL;
		}
		if (i < tokens.token_count && tokens.tokens[i].type == PIPE)
			i++;
	}
	tokens.env = export_variable_sep("LASTSTATUS", ft_itoa(last_status), tokens);
}
