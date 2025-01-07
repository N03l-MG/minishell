/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 08:27:00 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/07 15:52:33 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_quote_valid(const char *str)
{
	int	i;
	int	dquote;
	int	squote;
	int	open_quote;

	i = 0;
	dquote = -1;
	squote = -1;
	open_quote = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '"' && squote == -1)
		{
			dquote = -dquote;
			open_quote += dquote;
		}
		if (str[i] == '\'' && dquote == -1)
		{
			squote = -squote;
			open_quote += squote;
		}
		i ++;
	}
	return (open_quote);
}

static char	**ft_tokensplit(const char *str, int *splt_nmb)
{
	char	**ptr;
	int		i;
	int		x;
	int		totlen;

	if (str == NULL)
		return (NULL);
	i = 0;
	x = 0;
	totlen = ft_strlen(str);
	if (totlen == 0)
	{
		ptr = malloc(sizeof(char *));
		if (ptr == NULL)
			return (NULL);
		ptr[0] = 0;
		return (ptr);
	}
	ptr = malloc(sizeof(char *) * (spltnmb(str, totlen) + 1));
	if (ptr == NULL)
		return (NULL);
	while (str[i] == ' ')
		i ++;
	*splt_nmb = spltnmb(str, totlen);
	return (token_copy(ptr, str, i));
}

t_token	create_tokens(const char *input)
{
	t_token	tok;
	int		i;

	i = 0;
	tok.tokens = ft_tokensplit(input, &tok.token_count);
	if (check_quote_valid(input) != 0)
	{
		printf("Opened string never closed.");
		exit(1);
	}
	while (i < tok.token_count)
	{
		tok.tokens[i] = ft_tokentrim(tok.tokens[i], "\"'");
		i ++;
	}
	return (tok);
}
