/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 08:27:00 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/07 15:22:12 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_quote_closed(const char *str)
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
	int		totlen;

	if (str == NULL)
		return (NULL);
	i = 0;
	totlen = ft_strlen(str);
	if (totlen == 0)
	{
		ptr = malloc(sizeof(char *));
		if (ptr == NULL)
			return (NULL);
		ptr[0] = NULL;
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
	tok.is_string = malloc(sizeof(bool *) * tok.token_count);
	if (tok.is_string == NULL)
	{
		handle_error(MEMORY, "");
		free_tokens(tok.tokens, tok.token_count);
		exit(EXIT_FAILURE);
	}
	if (check_quote_closed(input) != 0)
		handle_error(INVALID_INPUT, tok.tokens[0]);
	while (i < tok.token_count)
	{
		tok.is_string[i] = false;
		if (tok.tokens[i][0] != '\'')
			tok.is_string[i] = true;
		tok.tokens[i] = ft_tokentrim(tok.tokens[i]);
		i ++;
	}
	return (tok);
}
