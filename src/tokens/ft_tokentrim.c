/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokentrim.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:42:54 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/17 15:26:53 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	find_start(char const *str)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(str);
	if ((str[0] != '\'' || str[j - 1] != '\'')
		&& (str[0] != '"' || str[j - 1] != '"'))
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i ++;
	}
	return (0);
}

static size_t	find_end(char const *str)
{
	int	i;

	i = ft_strlen(str);
	if ((str[0] != '\'' || str[i - 1] != '\'')
		&& (str[0] != '"' || str[i - 1] != '"'))
		return (i);
	while (i >= 0)
	{
		if (str[i] == '\'' || str[i] == '"')
			return (ft_strlen(str) - 1);
		i --;
	}
	return (ft_strlen(str));
}

char	*ft_tokentrim(char *str)
{
	char	*trimmed_s;
	char	*result;
	size_t	l;

	l = find_end(str) - find_start(str);
	trimmed_s = malloc(l + 1);
	if (!trimmed_s)
		return (NULL);
	ft_strlcpy(trimmed_s, &str[find_start(str)], l + 1);
	if (find_start(str) != 0)
		return (free(str), trimmed_s);
	free(str);
	result = ft_strtrim(trimmed_s, " ");
	free(trimmed_s);
	return (result);
}

int	check_quote_closed(t_input *tokens)
{
	int	i;
	int	j;
	int	dquote;
	int	squote;
	int	open_quote;

	i = 0;
	while (i < tokens->token_count)
	{
		dquote = -1;
		squote = -1;
		open_quote = 0;
		j = 0;
		while (tokens->tokens[i].token[j] != '\0')
		{
			if (tokens->tokens[i].token[j] == '"' && squote == -1)
			{
				dquote = -dquote;
				open_quote += dquote;
			}
			if (tokens->tokens[i].token[j] == '\'' && dquote == -1)
			{
				squote = -squote;
				open_quote += squote;
			}
			j ++;
			if (open_quote != 0)
			{
				handle_error(INVALID_INPUT, tokens->tokens[i].token);
				return (1);
			}
		}
		i ++;
	}
	return (0);
}
