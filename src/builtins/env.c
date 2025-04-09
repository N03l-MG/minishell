/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kixik   <github.com/kixikCodes>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:16:35 by jgraf             #+#    #+#             */
/*   Updated: 2025/02/06 18:59:20 by kixik            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*my_getenv(char **env, char *name)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], name, ft_strlen(name)) == 0
			&& env[i][ft_strlen(name)] == '=')
			return (env[i] + ft_strlen(name) + 1);
		i ++;
	}
	return (NULL);
}

int	my_getenv_index(char **env, char *name)
{
	int		i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], name, ft_strlen(name)) == 0
			&& env[i][ft_strlen(name)] == '=')
			return (i);
		i ++;
	}
	return (-1);
}

void	print_envs(t_input *tok)
{
	int		i;
	char	*equals_pos;

	i = 0;
	while (tok->env[i] != NULL)
	{
		equals_pos = ft_strchr(tok->env[i], '=');
		if (equals_pos && *(equals_pos + 1) != '\0')
			printf("%s\n", tok->env[i]);
		i++;
	}
	tok->last_status = 0;
}

static void	sort_envs(char **env)
{
	int		len;
	int		i;
	int		j;
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
}

void	print_sorted_env(char **env)
{
	int		i;
	char	*equals_pos;

	sort_envs(env);
	i = -1;
	while (env[++i])
	{
		equals_pos = ft_strchr(env[i], '=');
		if (equals_pos)
		{
			*equals_pos = '\0';
			if (*(equals_pos + 1) != '\0')
				printf("declare -x %s=\"%s\"\n", env[i], equals_pos + 1);
			else
				printf("declare -x %s\n", env[i]);
			*equals_pos = '=';
		}
		else
			printf("declare -x %s\n", env[i]);
	}
}
