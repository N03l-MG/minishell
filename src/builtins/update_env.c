/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:42:26 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/16 16:44:39 by nmonzon          ###   ########.fr       */
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

static void	create_and_add_new(char *env, char *name, char *con)
{
	int	i;
	int	j;

	i = 0;
	while (name[i] != '\0')
	{
		env[i] = name[i];
		i ++;
	}
	env[i++] = '=';
	j = 0;
	while (con[j] != '\0')
		env[i++] = con[j++];
	env[i] = '\0';
}

static char	*replace_and_add_new(char *repl, char *name, char *con, char *env)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (name[j] != '\0')
	{
		repl[j] = name[j];
		j ++;
	}
	repl[j++] = '=';
	while (con[k] != '\0')
		repl[j++] = con[k++];
	repl[j] = '\0';
	free(env);
	return (repl);
}

char	**add_envvar(t_input tok, char **environ, char *name, char *con)
{
	char	**env;
	int		i;

	env = malloc(sizeof(char *) * (get_entry_number(environ) + 2));
	if (env == NULL)
		handle_mem_error(&tok);
	i = 0;
	while (environ[i] != NULL)
	{
		env[i] = ft_strdup(environ[i]);
		i ++;
	}
	env[i] = malloc(ft_strlen(name) + ft_strlen(con) + 2);
	if (env[i] == NULL)
	{
		free(env);
		handle_mem_error(&tok);
	}
	create_and_add_new(env[i], name, con);
	env[i + 1] = NULL;
	free_env(environ);
	return (env);
}

char	**replace_envvar(t_input tok, char **env, char *name, char *con)
{
	int		i;
	char	*replacement;

	i = 0;
	while (ft_strncmp(env[i], name, ft_strlen(name)) != 0
		&& env[i] != NULL)
		i ++;
	if (env[i] == NULL)
		return (add_envvar(tok, env, name, con));
	replacement = malloc(ft_strlen(name) + ft_strlen(con) + 2);
	if (replacement == NULL)
		handle_mem_error(&tok);
	env[i] = replace_and_add_new(replacement, name, con, env[i]);
	return (env);
}
