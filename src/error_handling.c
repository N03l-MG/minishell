/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:39:02 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/07 15:54:45 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_error(t_error error, char *current_token)
{
	if (error == ENV_NOT_FOUND)
		ft_fprintf(2, "Error: PATH environment variable not found.\n");
	else if (error == COMMAND_NOT_FOUND)
		ft_fprintf(2, "Error: %s: Command not found.\n", current_token);
	else if (error == INVALID_INPUT)
		ft_fprintf(2, "Error: %s: Invalid input.\n", current_token);
	else if (error == MEMORY)
		ft_fprintf(2, "Error: Memory allocation failed.\n");
	return (1);
}
