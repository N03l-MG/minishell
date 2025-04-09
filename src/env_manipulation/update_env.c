/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kixik   <github.com/kixikCodes>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:42:26 by jgraf             #+#    #+#             */
/*   Updated: 2025/02/03 16:18:39 by kixik            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_entry_number(char **environ)
{
	int	n;

	n = 0;
	while (environ[n] != NULL)
		n++;
	return (n);
}

void	create_and_add_new(char *env, char *name, char *con)
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

char	*replace_and_add_new(char *repl, char *name, char *con, char *env)
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

char	**add_envvar(t_input tok, char *name, char *con)
{
	char	**env;
	int		i;

	env = malloc(sizeof(char *) * (get_entry_number(tok.env) + 2));
	if (env == NULL)
		handle_fatal_error(MEMORY_ERROR, NULL, &tok);
	i = 0;
	while (tok.env[i] != NULL)
	{
		env[i] = ft_strdup(tok.env[i]);
		i ++;
	}
	env[i] = malloc(ft_strlen(name) + ft_strlen(con) + 2);
	if (env[i] == NULL)
	{
		free(env);
		handle_fatal_error(MEMORY_ERROR, NULL, &tok);
	}
	create_and_add_new(env[i], name, con);
	env[i + 1] = NULL;
	free_env(tok.env);
	return (env);
}

char	**replace_envvar(t_input tok, char *name, char *con)
{
	int		i;
	char	*replacement;

	i = my_getenv_index(tok.env, name);
	replacement = malloc(ft_strlen(name) + ft_strlen(con) + 2);
	if (replacement == NULL)
		handle_fatal_error(MEMORY_ERROR, NULL, &tok);
	tok.env[i] = replace_and_add_new(replacement, name, con, tok.env[i]);
	return (tok.env);
}
