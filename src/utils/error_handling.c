/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:39:02 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/09 17:00:02 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_error(t_error error, char *current)
{
	if (error == ENV_NOT_FOUND)
		ft_fprintf(2, "Error: PATH environment variable not found.\n");
	else if (error == COMMAND_NOT_FOUND)
		ft_fprintf(2, "Error: %s: Command not found.\n", current);
	else if (error == INVALID_INPUT)
		ft_fprintf(2, "Error: %s: Invalid input.\n", current);
	else if (error == INVALID_FILE)
		ft_fprintf(2, "Error: %s: No such file or directory.", current);
	else if (error == PIPE)
		ft_fprintf(2, "Error: Pipe failure.\n");
	else if (error == FORK)
		ft_fprintf(2, "Error: Fork failure.\n");
	else if (error == EXEC)
		ft_fprintf(2, "Error: %s: Failed to execute command.\n", current);
	return (1);
}

void	handle_mem_error(t_token *tokens)
{
	ft_fprintf(2, "Fatal Error: Memory allocation failed.\n");
	free_allocated(NULL, tokens, MEMORY);
	exit(EXIT_FAILURE);
}

void	free_allocated(t_data *data, t_token *tokens, t_error error)
{
	if (error == MEMORY)
		free_tokens(tokens->tokens, tokens->token_count);
	if (data->cmd && data->full_path)
	{
		free(data->cmd);
		free(data->full_path);
	}
}

void	free_check_char(char *str)
{
	if (str != NULL)
		free(str);
}
