/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:42:26 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/13 16:42:27 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_entry_number(char **environ)
{
	int	n;

	n = 0;
	while (environ[n] != NULL)
		n ++;
	return (n);
}

void	free_env(char **env)
{
	int	i;

	i = 0;
	while (i < get_entry_number(env))
	{
		free(env[i]);
		i ++;
	}
	free(env);
}

char	**init_env(char **environ)
{
	char	**env;
	int		i;

	env = malloc(sizeof(char *) * (get_entry_number(environ) + 1));
	if (env == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (environ[i] != NULL)
	{
		env[i] = ft_strdup(environ[i]);
		if (env[i] == NULL)
		{
			free_env(env);
			exit(EXIT_FAILURE);
		}
		i ++;
	}
	env[i] = NULL;
	return (env);
}
