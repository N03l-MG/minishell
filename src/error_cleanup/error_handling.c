/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:39:02 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/06 13:01:44 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_error_message(t_error error)
{
	static char	*messages[] = {
	[ENV_NOT_FOUND] = RED "Environment variable not found" RESET,
	[COMMAND_NOT_FOUND] = RED "Command not found" RESET,
	[INVALID_INPUT] = RED "Invalid input" RESET,
	[INVALID_FILE] = RED "No such file or directory" RESET,
	[PIPE_ERROR] = RED "Pipe failure" RESET,
	[FORK_ERROR] = RED "Fork failure" RESET,
	[EXEC_ERROR] = RED "Failed to execute command" RESET,
	[MEMORY_ERROR] = RED "Memory allocation failed" RESET,
	[PERMISSION_ERROR] = RED "Permission denied" RESET,
	[SYNTAX_ERROR] = RED "Parse error" RESET
	};

	return (messages[error]);
}

static int	get_error_status(t_error error)
{
	static const int	status[] = {
	[ENV_NOT_FOUND] = 1,
	[COMMAND_NOT_FOUND] = 127,
	[INVALID_INPUT] = 1,
	[INVALID_FILE] = 1,
	[PIPE_ERROR] = 1,
	[FORK_ERROR] = 1,
	[EXEC_ERROR] = 126,
	[MEMORY_ERROR] = 1,
	[PERMISSION_ERROR] = 126,
	[SYNTAX_ERROR] = 2
	};

	return (status[error]);
}

void	print_error(t_error error, char *context)
{
	ft_putstr_fd("minishell: ", 2);
	if (context)
	{
		ft_putstr_fd(context, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(get_error_message(error), 2);
}

int	handle_error(t_error error, char *context, t_input *tok)
{
	int		status;

	status = get_error_status(error);
	print_error(error, context);
	if (tok)
		tok->last_status = status;
	return (status);
}

void	handle_fatal_error(t_error error, char *context, t_input *tokens)
{
	handle_error(error, context, tokens);
	clean_exit(tokens, NULL);
}
