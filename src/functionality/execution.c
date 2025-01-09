/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:03:49 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/09 13:47:01 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_input(t_token tokens)
{
	t_data	data;
	int		i;
	int		cmd_start;
	pid_t	pid;

	data.prev_fd = -1;
	i = 0;
	while (i < tokens.token_count)
	{
		cmd_start = i;
		set_i(&i, &tokens);
		data.cmd = parse_command(tokens, cmd_start, i);
		data.full_path = resolve_command_path(data.cmd[0]);
		if (!(i == tokens.token_count))
			setup_pipe(data.pipe_fds);
		pid = fork();
		if (pid == -1)
			handle_error(FORK, NULL);
		else if (pid == 0)
			handle_child(&data, (i == tokens.token_count));
		else
			handle_parent(&data, &data.prev_fd, pid, (i == tokens.token_count));
		free_allocated(&data, &tokens, NOTANERROR);
		i++;
	}
}
