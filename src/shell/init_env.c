/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:42:26 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/20 15:46:13 by nmonzon          ###   ########.fr       */
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
	char	*shlvl_str;

	env = malloc(sizeof(char *) * (get_entry_number(environ) + 3));
	if (!env)
		handle_fatal_error(MEMORY_ERROR, NULL, NULL);
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
	if (!(env = add_envvar_pre(env, "LASTSTATUS", "0")))
		handle_fatal_error(MEMORY_ERROR, NULL, NULL);
	shlvl_str = ft_itoa(ft_atoi(my_getenv(env, "SHLVL")) + 1);
	if (!shlvl_str)
		handle_fatal_error(MEMORY_ERROR, NULL, NULL);
	env = replace_envvar_pre(env, "SHLVL", shlvl_str);
	free(shlvl_str);
	if (!env)
		handle_fatal_error(MEMORY_ERROR, NULL, NULL);
	return (env);
}

char	**add_envvar_pre(char **env, char *name, char *con)
{
	char	**env_new;
	int		i;

	env_new = malloc(sizeof(char *) * (get_entry_number(env) + 2));
	i = 0;
	while (env[i] != NULL)
	{
		env_new[i] = ft_strdup(env[i]);
		i ++;
	}
	env_new[i] = malloc(ft_strlen(name) + ft_strlen(con) + 2);
	create_and_add_new(env_new[i], name, con);
	env_new[i + 1] = NULL;
	return (env_new);
}

char	**replace_envvar_pre(char **env, char *name, char *con)
{
	int		i;
	char	*replacement;

	i = 0;
	while (ft_strncmp(env[i], name, ft_strlen(name)) != 0
		&& env[i] != NULL)
		i ++;
	replacement = malloc(ft_strlen(name) + ft_strlen(con) + 2);
	env[i] = replace_and_add_new(replacement, name, con, env[i]);
	return (env);
}
