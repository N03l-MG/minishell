/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:42:26 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/27 15:36:47 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

static char	**init_env_vars(char **env, char **environ)
{
	int	i;

	i = 0;
	while (environ[i])
	{
		env[i] = ft_strdup(environ[i]);
		if (!env[i])
		{
			free_env(env);
			handle_fatal_error(MEMORY_ERROR, NULL, NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

static char	**add_default_vars(char **env)
{
	char	*shlvl_str;

	shlvl_str = ft_itoa(ft_atoi(my_getenv(env, "SHLVL")) + 1);
	if (!shlvl_str)
		handle_fatal_error(MEMORY_ERROR, NULL, NULL);
	env = replace_envvar_pre(env, "SHLVL", shlvl_str);
	free(shlvl_str);
	if (!env)
		handle_fatal_error(MEMORY_ERROR, NULL, NULL);
	return (env);
}

char	**init_env(char **environ)
{
	char	**env;

	env = malloc(sizeof(char *) * (get_entry_number(environ) + 2));
	if (!env)
		handle_fatal_error(MEMORY_ERROR, NULL, NULL);
	env = init_env_vars(env, environ);
	env = add_default_vars(env);
	return (env);
}
