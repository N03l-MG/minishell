/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:03:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/10 13:33:40 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void get_redir(t_token tokens, int *cmd_start, int *cmd_end, char **infile, char **outfile)
{
	int	i;

	i = *cmd_start;
	*infile = NULL;
	*outfile = NULL;
	while (i < *cmd_end)
	{
		if (ft_strcmp(tokens.tokens[i], ">") == 0 || ft_strcmp(tokens.tokens[i], "<") == 0)
		{
			if (i + 1 < *cmd_end)
			{
				if (ft_strcmp(tokens.tokens[i], ">") == 0)
					*outfile = tokens.tokens[i + 1];
				else if (ft_strcmp(tokens.tokens[i], "<") == 0)
					*infile = tokens.tokens[i + 1];
				for (int j = i; j + 2 < *cmd_end; j++)
					tokens.tokens[j] = tokens.tokens[j + 2];
				*cmd_end -= 2;
			}
			else
			{
				handle_error(INVALID_INPUT, tokens.tokens[i]);
			}
		}
		else
			i++;
	}
}

void	handle_redirections(char *input_file, char *output_file)
{
	int	fd;

	if (input_file)
	{
		fd = open(input_file, O_RDONLY);
		if (fd == -1)
			handle_error(INVALID_FILE, input_file);
		dup2(fd, 0);
		close(fd);
	}
	if (output_file)
	{
		fd = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			handle_error(INVALID_FILE, output_file);
		dup2(fd, 1);
		close(fd);
	}
}

void	execute_input(t_token tokens)
{
	t_data	data;
	int		i;
	int		cmd_start;
	pid_t	pid;
	char	*infile;
	char	*outfile;

	data.prev_fd = -1;
	i = 0;
	while (i < tokens.token_count)
	{
		cmd_start = i;
		set_i(&i, &tokens);
		infile = NULL;
		outfile = NULL;
		get_redir(tokens, &cmd_start, &i, &infile, &outfile);
		data.cmd = parse_command(tokens, cmd_start, i);
		data.full_path = resolve_command_path(data.cmd[0]);
		if (!(i == tokens.token_count))
			setup_pipe(data.pipe_fds);
		pid = fork();
		if (pid == -1)
			handle_error(FORK, NULL);
		else if (pid == 0)
			handle_child(&data, (i == tokens.token_count), infile, outfile);
		else
			handle_parent(&data, &data.prev_fd, pid, (i == tokens.token_count));
		free_allocated(&data, &tokens, NOTANERROR);
		i++;
	}
}
