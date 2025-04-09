/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kixik   <github.com/kixikCodes>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:46:50 by jgraf             #+#    #+#             */
/*   Updated: 2025/02/06 20:03:56 by kixik            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_check_char(char *str)
{
	if (str != NULL)
		free(str);
}

static char	*get_var_name(char *str)
{
	int		i;
	int		len;
	char	*var;

	i = 0;
	len = 0;
	while (str[len] != '=' && str[len] != '\0')
		len++;
	var = malloc(len + 1);
	if (var == NULL)
		return (NULL);
	while (i < len)
	{
		var[i] = str[i];
		i++;
	}
	var[i] = '\0';
	return (var);
}

static char	*get_var_con(char *str)
{
	int		len;
	int		i;
	char	*var;

	len = 0;
	while (str[len] != '=')
	{
		if (str[len++] == '\0')
			return (ft_strdup(""));
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
		var[len++] = str[i++];
	var[len] = '\0';
	return (var);
}

char	**export_variable(char *var, t_input *tok)
{
	char	*var_name;
	char	*var_con;
	char	*temp;

	var_name = get_var_name(var);
	if (ft_strchr(var, '='))
		var_con = get_var_con(var);
	else
	{
		temp = ft_strjoin(var, "=");
		var_con = ft_strdup("");
		free(temp);
	}
	if (var_name[0] == '\0')
		return (free(var_name),
			handle_error(INVALID_INPUT, var, tok), tok->env);
	if (!check_valid_export_unset(var_name))
		return (handle_error(INVALID_INPUT, var, tok), tok->env);
	if (my_getenv(tok->env, var_name))
		tok->env = replace_envvar(*tok, var_name, var_con);
	else
		tok->env = add_envvar(*tok, var_name, var_con);
	return (free(var_name), free(var_con), tok->env);
}

char	**export_variable_sep(char *var, char *con, t_input tok)
{
	char	*var_con;

	var_con = ft_strtrim(con, "\"'");
	if (var_con == NULL)
	{
		free(var_con);
		handle_fatal_error(MEMORY_ERROR, NULL, &tok);
	}
	if (my_getenv(tok.env, var) == NULL)
		tok.env = add_envvar(tok, var, var_con);
	else
		tok.env = replace_envvar(tok, var, var_con);
	return (free(var_con), tok.env);
}
