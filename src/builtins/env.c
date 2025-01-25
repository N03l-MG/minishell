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

void	print_sorted_env(char **env)
{
	int		i;
	int		j;
	int		len;
	char	*tmp;

	len = 0;
	while (env[len])
		len++;
	i = -1;
	while (++i < len - 1)
	{
		j = -1;
		while (++j < len - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
		}
	}
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
}
