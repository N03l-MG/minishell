/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:33:19 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/20 13:47:07 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_i(int *i, t_input *tokens)
{
	while (*i < tokens->token_count
		&& ft_strcmp(tokens->tokens[*i].token, "|") != 0)
		(*i)++;
}

char	**parse_command(t_input tokens, int cmd_start, int cmd_end)
{
	char	**cmd;
	int		i;
	int		j;
	int		valid_tokens;

	valid_tokens = 0;
	i = cmd_start;
	while (i < cmd_end)
	{
		if (tokens.tokens[i].type == REDIR_IN
			|| tokens.tokens[i].type == REDIR_OUT
			|| tokens.tokens[i].type == REDIR_APPEND)
		{
			i++;
			continue ;
		}
		if (tokens.tokens[i].type == STRING)
			valid_tokens++;
		i++;
	}
	if (valid_tokens == 0)
		return (NULL);
	cmd = malloc((valid_tokens + 1) * sizeof(char *));
	if (!cmd)
		return (NULL);
	j = 0;
	i = cmd_start;
	while (i < cmd_end && j < valid_tokens)
	{
		if (tokens.tokens[i].type == REDIR_IN
			|| tokens.tokens[i].type == REDIR_OUT
			|| tokens.tokens[i].type == REDIR_APPEND)
		{
			i++;
			continue ;
		}
		if (tokens.tokens[i].type == STRING)
		{
			cmd[j] = ft_strdup(tokens.tokens[i].token);
			if (!cmd[j])
			{
				while (--j >= 0)
					free(cmd[j]);
				free(cmd);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	cmd[j] = NULL;
	return (cmd);
}

void	setup_pipe(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
		handle_error(PIPE_ERROR, NULL, NULL);
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
