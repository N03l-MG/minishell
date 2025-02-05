/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:20:20 by jgraf             #+#    #+#             */
/*   Updated: 2025/02/05 16:26:22 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Allowed by the subject!
static volatile sig_atomic_t	g_sigint_received = 0;

void	sig_sigint(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	rl_replace_line("", 0);
	ft_fprintf(STDERR_FILENO, "\n");
	if (waitpid(-1, NULL, WNOHANG) == 0)
		return ;
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
