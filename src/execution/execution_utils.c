/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:33:19 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/24 11:54:54 by nmonzon          ###   ########.fr       */
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
			|| tokens.tokens[i].type == REDIR_APPEND)
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
			|| args.tokens.tokens[i].type == REDIR_APPEND)
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

char	**parse_command(t_input tokens, int cmd_start, int cmd_end)
{
	char			**cmd;
	int				valid_tokens;

	valid_tokens = parse_part_1(cmd_start, cmd_end, tokens);
	if (valid_tokens == 0)
		return (NULL);
	cmd = malloc((valid_tokens + 1) * sizeof(char *));
	if (!cmd)
		return (NULL);
	cmd = parse_part_2((t_parse_args){cmd_start, cmd_end, tokens,
			cmd, valid_tokens});
	cmd[valid_tokens] = NULL;
	return (cmd);
}

void	setup_pipe(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
		handle_error(PIPE_ERROR, NULL, NULL);
}
