/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 08:27:00 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/13 15:59:15 by nmonzon          ###   ########.fr       */
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

t_input	create_tokens(const char *input)
{
	t_input	tok;
	int		i;
	char	**split_tokens;

	i = 0;
	split_tokens = ft_tokensplit(input, &tok.token_count);
	tok.tokens = malloc(sizeof(t_token) * tok.token_count);
	if (tok.tokens == NULL)
	{
		handle_error(MEMORY_ERROR, "");
		exit(EXIT_FAILURE);
	}
	if (check_quote_closed(input) != 0)
		handle_error(INVALID_INPUT, split_tokens[0]);
	while (i < tok.token_count)
	{
		tok.tokens[i].token = ft_tokentrim(split_tokens[i]);
		if (ft_strcmp(tok.tokens[i].token, "|") == 0)
			tok.tokens[i].type = PIPE;
		else if (ft_strcmp(tok.tokens[i].token, "<") == 0)
			tok.tokens[i].type = REDIR_IN;
		else if (ft_strcmp(tok.tokens[i].token, ">") == 0)
			tok.tokens[i].type = REDIR_OUT;
		else if (ft_strcmp(tok.tokens[i].token, ">>") == 0)
			tok.tokens[i].type = REDIR_APPEND;
		else if (ft_strcmp(tok.tokens[i].token, ";") == 0)
			tok.tokens[i].type = SEMICOLON;
		else
			tok.tokens[i].type = STRING;
		i++;
	}
	free(split_tokens);
	return (tok);
}
