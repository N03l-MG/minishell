/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:46:22 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/27 11:05:39 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**unset_envvar(t_input tok, char **environ, char *name)
{
	char	**env;
	int		i;
	int		j;

	env = malloc(sizeof(char *) * get_entry_number(environ));
	if (env == NULL)
		handle_fatal_error(MEMORY_ERROR, NULL, &tok);
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

char	**unset_variable(char *var_name, t_input tok)
{
	char	**env;

	if (my_getenv(tok.env, var_name) == NULL)
		return (tok.env);
	env = unset_envvar(tok, tok.env, var_name);
	free_env(tok.env);
	return (env);
}
