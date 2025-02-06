/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:33:19 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/06 13:55:05 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_i(int *i, t_input *tokens)
{
	while (*i < tokens->token_count
		&& ft_strcmp(tokens->tokens[*i].token, "|") != 0)
		(*i)++;
}

int	parse_part_1(int cmd_start, int cmd_end, t_input tokens)
{
	int	i;
	int	valid_tokens;

	valid_tokens = 0;
	i = cmd_start;
	while (i < cmd_end)
	{
		if (tokens.tokens[i].type == REDIR_IN
			|| tokens.tokens[i].type == REDIR_OUT
			|| tokens.tokens[i].type == APPEND)
		{
			i++;
			continue ;
		}
		if (tokens.tokens[i].type == STRING)
			valid_tokens++;
		i++;
	}
	return (valid_tokens);
}

char	**parse_part_2(t_parse_args args)
{
	int	j;
	int	i;

	j = 0;
	i = args.cmd_start;
	while (i < args.cmd_end && j < args.valid_tokens)
	{
		if (args.tokens.tokens[i].type == REDIR_IN
			|| args.tokens.tokens[i].type == REDIR_OUT
			|| args.tokens.tokens[i].type == APPEND)
		{
			i++;
			continue ;
		}
		if (args.tokens.tokens[i].type == STRING)
		{
			args.cmd[j] = ft_strdup(args.tokens.tokens[i].token);
			if (!args.cmd[j])
				handle_fatal_error(MEMORY_ERROR, NULL, &args.tokens);
			j++;
		}
		i++;
	}
	return (args.cmd);
}

char	**parse_cmd(t_input tokens, int cmd_start, int cmd_end)
{
	char	**cmd;
	int		valid_tokens;
	char	**result;

	valid_tokens = parse_part_1(cmd_start, cmd_end, tokens);
	if (valid_tokens == 0)
		return (NULL);
	cmd = malloc((valid_tokens + 1) * sizeof(char *));
	if (!cmd)
		return (NULL);
	result = parse_part_2((t_parse_args){cmd_start, cmd_end, tokens,
			cmd, valid_tokens});
	if (!result)
	{
		free_cmd_array(cmd);
		return (NULL);
	}
	result[valid_tokens] = NULL;
	return (result);
}

void	setup_pipe(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
		handle_error(PIPE_ERROR, NULL, NULL);
}
