/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_variables.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:02:01 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/28 16:36:14 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_variable_content(t_input tok, char *input, int *i)
{
	char	result[4096];
	int		j;

	j = 0;
	if (input[*i] == '?')
		return ((*i)++, ft_strdup(ft_itoa(tok.last_status)));
	if (!ft_isalnum(input[*i]) && input[*i] != '_')
		return (ft_strdup("$"));
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		result[j++] = input[(*i)++];
	result[j] = '\0';
	if (my_getenv(tok.env, result) == NULL)
		return (ft_strdup(""));
	return (ft_strdup(my_getenv(tok.env, result)));
}

static char	*append_var(t_input tok, char *old_input, char *input, int *i)
{
	char	*new_input;
	char	*var_con;
	int		j;

	j = 0;
	var_con = get_variable_content(tok, input, i);
	new_input = malloc(ft_strlen(old_input) + ft_strlen(var_con) + 1);
	while (old_input[j] != '\0')
	{
		new_input[j] = old_input[j];
		j ++;
	}
	new_input[j] = '\0';
	new_input = ft_strjoin(new_input, var_con);
	return (free(var_con), free(old_input), new_input);
}

static char	*copy_until_var(char *old_input, char *input, int *i, int res_stat)
{
	char		new_input[4096];
	static int	in_dquote = 0;
	static int	in_squote = 0;
	int			j;

	j = 0;
	if (res_stat)
	{
		in_dquote = 0;
		in_squote = 0;
	}
	while (old_input[j] != '\0')
	{
		new_input[j] = old_input[j];
		j ++;
	}
	while (input[*i] != '\0')
	{
		if (input[*i] == '$' && !in_squote)
			break ;
		if (input[*i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		if (input[*i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		new_input[j++] = input[(*i)++];
	}
	new_input[j] = '\0';
	return (free(old_input), ft_strdup(new_input));
}

char	*replace_env(t_input token, char *input)
{
	char	*new_input;
	int		i;
	int		reset_static;

	new_input = malloc(1);
	reset_static = 1;
	if (new_input == NULL)
	{
		free(input);
		handle_fatal_error(MEMORY_ERROR, NULL, &token);
	}
	new_input[0] = '\0';
	i = 0;
	while (input[i] != '\0')
	{
		new_input = copy_until_var(new_input, input, &i, reset_static);
		reset_static = 0;
		if (input[i] == '\0')
			break ;
		i ++;
		new_input = append_var(token, new_input, input, &i);
	}
	return (free(input), new_input);
}
