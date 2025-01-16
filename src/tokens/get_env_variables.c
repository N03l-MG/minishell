/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_variables.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:02:01 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/16 17:19:38 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_len(char *str, int i)
{
	while (str[i] != '\0' && (str[i] != '$' || !ft_isalpha(str[i + 1])))
		i ++;
	return (i);
}

static int	get_varlen(t_input t, char *str, int i, char *var_name)
{
	int	j;

	j = 0;
	while (ft_isalpha(str[i]))
	{
		var_name[j] = str[i];
		i ++;
		j ++;
	}
	var_name[j] = '\0';
	if (my_getenv(t.env, var_name) != NULL)
		return (ft_strlen(my_getenv(t.env, var_name)));
	return (0);
}

static char	*copy_envvar(char *old_tok, char *tok, int *i, t_input t)
{
	char	*new_tok;
	char	name[1024];
	char	*con;
	int		j;
	int		v;

	j = 0;
	v = 0;
	new_tok = malloc(ft_strlen(old_tok) + get_varlen(t, tok, *i, name) + 1);
	if (new_tok == NULL)
		handle_mem_error(&t);
	con = my_getenv(t.env, name);
	while (old_tok[j] != '\0')
	{
		new_tok[j] = old_tok[j];
		j ++;
	}
	while (con != NULL && con[v] != '\0')
		new_tok[j++] = con[v++];
	(*i) += ft_strlen(name);
	new_tok[j] = '\0';
	return (free(old_tok), new_tok);
}

static char	*copy_string(char *old_tok, char *tok, int *i, t_input token)
{
	char	*new_tok;
	int		j;

	j = 0;
	new_tok = malloc(ft_strlen(old_tok) + get_len(tok, *i) + 1);
	if (new_tok == NULL)
		handle_mem_error(&token);
	while (old_tok[j] != '\0')
	{
		new_tok[j] = old_tok[j];
		j ++;
	}
	while (tok[*i] != '\0'
		&& (tok[(*i)] != '$' || !ft_isalpha(tok[(*i) + 1])))
	{
		new_tok[j] = tok[*i];
		j ++;
		(*i)++;
	}
	new_tok[j] = '\0';
	return (free(old_tok), new_tok);
}

char	*replace_env(t_input token, char *tok)
{
	char	*new_tok;
	int		i;
	int		len;

	len = 0;
	new_tok = malloc(1);
	if (new_tok == NULL)
		handle_mem_error(&token);
	new_tok[0] = '\0';
	i = 0;
	while (tok[i] != '\0')
	{
		new_tok = copy_string(new_tok, tok, &i, token);
		if (tok[i] == '\0')
			break ;
		i ++;
		if (tok[i - 1] == '$' && ft_isalpha(tok[i]))
			new_tok = copy_envvar(new_tok, tok, &i, token);
	}
	return (free(tok), new_tok);
}
