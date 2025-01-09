/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:03:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/09 17:08:36 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_redir(t_token tokens, int *cmd_start, int *cmd_end, char *type)
{
	char	*filename;
	int		i;

	filename = NULL;
	i = *cmd_start;
	while (i < *cmd_end)
	{
		if (ft_strcmp(tokens.tokens[i], ">") == 0
			|| ft_strcmp(tokens.tokens[i], "<") == 0)
		{
			*type = tokens.tokens[i][0];
			if (i + 1 < *cmd_end)
				filename = tokens.tokens[i + 1];
			*cmd_end -= 2;
			break ;
		}
		i++;
	}
	return (filename);
}

void	handle_redirections(char *input_file, char *output_file)
{
	int	fd;

	if (input_file)
	{
		fd = open(input_file, O_RDONLY);
		if (fd == -1)
			handle_error(INVALID_FILE, input_file);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (output_file)
	{
		fd = open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	execute_input(t_token tokens)
{
	t_data	data;
	int		i;
	int		cmd_start;
	pid_t	pid;
	char	*file;
	char	*infile;
	char	*outfile;
	char	redirection_type;

	data.prev_fd = -1;
	i = 0;
	while (i < tokens.token_count)
	{
		cmd_start = i;
		set_i(&i, &tokens);
		redirection_type = '\0';
		infile = NULL;
		outfile = NULL;
		if (ft_strchr(tokens.tokens[cmd_start], '>')
			|| ft_strchr(tokens.tokens[cmd_start], '<'))
		{
			file = get_redir(tokens, &cmd_start, &i, &redirection_type);
			if (redirection_type == '>')
				outfile = file;
			else if (redirection_type == '<')
				infile = file;
		}
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
