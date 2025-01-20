/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:39:02 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/20 15:11:57 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **tokens, int count)
{
	int	i;

	if (!tokens || !*tokens)
		return ;
	i = 0;
	while (i <= count)
	{
		if ((*tokens)[i].token)
		{
			free((*tokens)[i].token);
			(*tokens)[i].token = NULL;
		}
		i++;
	}
	free(*tokens);
	*tokens = NULL;
}

int	handle_error(t_error error, char *current, t_input *tok)
{
	int	status;

	status = 0;
	if (error == ENV_NOT_FOUND)
	{
		ft_fprintf(2, "Error: %s: Environment variable not found.\n", current);
		status = 1;
	}
	else if (error == COMMAND_NOT_FOUND)
	{
		ft_fprintf(2, "Error: %s: Command not found.\n", current);
		status = 127;
	}
	else if (error == INVALID_INPUT)
	{
		ft_fprintf(2, "Error: %s: Invalid input.\n", current);
		status = 1;
	}
	else if (error == INVALID_FILE)
	{
		ft_fprintf(2, "Error: %s: No such file or directory.\n", current);
		status = 1;
	}
	else if (error == PIPE_ERROR)
	{
		ft_fprintf(2, "Error: Pipe failure.\n");
		status = 1;
	}
	else if (error == FORK_ERROR)
	{
		ft_fprintf(2, "Error: Fork failure.\n");
		status = 1;
	}
	else if (error == EXEC_ERROR)
	{
		ft_fprintf(2, "Error: %s: Failed to execute command.\n", current);
		status = 256;
	}
	tok->env = export_variable_sep("LASTSTATUS", ft_itoa(status), *tok);
	return (1);
}

void	handle_mem_error(t_input *tokens)
{
	ft_fprintf(2, "Fatal Error: Memory allocation failed.\n");
	free_allocated(NULL, tokens, MEMORY_ERROR);
	exit(EXIT_FAILURE);
}

void	free_allocated(t_data *data, t_input *tokens, t_error error)
{
	if (error == MEMORY_ERROR || error == EXEC_ERROR)
		free_tokens(&tokens->tokens, tokens->token_count);
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
