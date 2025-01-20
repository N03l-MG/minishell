/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:16:35 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/20 16:18:03 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_envs(t_input *tok)
{
	int	i;

	i = 0;
	while (tok->env[i] != NULL)
	{
		printf("%s\n", tok->env[i]);
		i ++;
	}
	tok->env = export_variable_sep("LASTSTATUS", "0", *tok);
}
