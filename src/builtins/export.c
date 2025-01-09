/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:46:50 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/08 14:46:51 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_name(char *str)
{
	int		i;
	int		len;
	char	*var;

	i = 0;
	len = 0;
	while (str[len] != '=')
	{
		if (str[len] == '\0')
			return (NULL);
		len ++;
	}
	var = malloc(len + 1);
	if (var == NULL)
		return (NULL);
	while (str[i] != '=')
	{
		var[i] = str[i];
		i ++;
	}
	var[i] = '\0';
	return (var);
}

static char	*get_var_con(char *str)
{
	int		len;
	int		i;
	char	*var;

	i = 0;
	len = 0;
	while (str[len] != '=')
	{
		if (str[len] == '\0')
			return (NULL);
		len ++;
	}
	i = len + 1;
	len = 0;
	while (str[len] != '\0')
		len ++;
	var = malloc(len + 1);
	if (var == NULL)
		return (NULL);
	len = 0;
	while (str[i] != '\0')
	{
		var[len] = str[i];
		i ++;
		len ++;
	}
	var[len] = '\0';
	return (var);
}

void	export_variable(char **env, char *var)
{
	char	*var_name;
	char	*var_con;
	char	*content;

	(void)env;
	var_name = get_var_name(var);
	content = get_var_con(var);
	var_con = ft_strtrim(content, "\"'");
	free(content);
	if (var_name == NULL || var_con == NULL)
	{
		handle_error(MEMORY, "");
		free_check_char(var_name);
		free_check_char(var_con);
	}
	printf("%s\n%s\n", var_name, var_con);
	free(var_name);
	free(var_con);
}
