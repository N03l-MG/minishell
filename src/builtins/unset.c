/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:46:22 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/16 16:52:11 by nmonzon          ###   ########.fr       */
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

static char	**unset_envvar(t_input tok, char **environ, char *name)
{
	char	**env;
	int		i;
	int		j;

	env = malloc(sizeof(char *) * get_entry_number(environ));
	if (env == NULL)
		handle_mem_error(&tok);
	i = 0;
	j = 0;
	while (environ[i] != NULL)
	{
		if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0)
		{
			i ++;
			continue ;
		}
		env[j] = ft_strdup(environ[i]);
		i ++;
		j ++;
	}
	env[j] = NULL;
	return (env);
}

char	**unset_variable(char **environ, char *var_name, t_input tok)
{
	char	**env;

	if (my_getenv(environ, var_name) == NULL)
		return (environ);
	env = unset_envvar(tok, environ, var_name);
	free_env(environ);
	return (env);
}
