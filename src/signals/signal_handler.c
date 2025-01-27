/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:20:20 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/27 15:18:43 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Allowed by the subject!
static volatile sig_atomic_t g_sigint_received = 0;

void	sig_sigint(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	ft_fprintf(STDERR_FILENO, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

bool	get_and_reset_sigint(void)
{
	int	was_received;

	was_received = g_sigint_received;
	g_sigint_received = 0;
	return (was_received);
}
