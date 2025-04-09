/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_trim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kixik   <github.com/kixikCodes>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:42:54 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/27 17:04:48 by kixik            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	quote_shananigans(char *str, int *in_dquote, int *in_squote, int *i)
{
	if (str[*i] == '"' && !*in_squote)
	{
		*in_dquote = !*in_dquote;
		(*i)++;
		return (1);
	}
	if (str[*i] == '\'' && !*in_dquote)
	{
		*in_squote = !*in_squote;
		(*i)++;
		return (1);
	}
	return (0);
}

char	*ft_tokentrim(char *str)
{
	char	*result;
	int		in_dquote;
	int		in_squote;
	int		i;
	int		j;

	i = 0;
	j = 0;
	in_dquote = 0;
	in_squote = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i] != '\0')
	{
		if (quote_shananigans(str, &in_dquote, &in_squote, &i) == 1)
			continue ;
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (free(str), result);
}

bool	check_unclosed_quotes(t_input *token, const char *input)
{
	int	i;
	int	dquote;
	int	squote;

	i = 0;
	dquote = 0;
	squote = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '"' && !squote)
			dquote = !dquote;
		if (input[i] == '\'' && !dquote)
			squote = !squote;
		i ++;
	}
	if (dquote || squote)
	{
		handle_error(INVALID_INPUT, NULL, token);
		return (true);
	}
	return (false);
}
