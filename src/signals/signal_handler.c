/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 14:20:20 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/07 15:14:34 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_sigint(int sig)
{
	(void)sig;
	ft_fprintf(STDERR_FILENO, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
