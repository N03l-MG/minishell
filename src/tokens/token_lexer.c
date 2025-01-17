/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:14:17 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/17 15:14:36 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';' || c == ' ');
}

static t_ttype	get_token_type(const char *str)
{
	if (!str || !*str)
		return (END);
	if (!strcmp(str, "|"))
		return (PIPE);
	if (!strcmp(str, "<"))
		return (REDIR_IN);
	if (!strcmp(str, ">"))
		return (REDIR_OUT);
	if (!strcmp(str, ">>"))
		return (REDIR_APPEND);
	if (!strcmp(str, ";"))
		return (SEMICOLON);
	return (STRING);
}

static int	count_tokens(const char *input)
{
	int		count;
	int		i;
	bool	in_quotes;
	char	quote_char;

	count = 0;
	i = 0;
	in_quotes = false;
	quote_char = 0;
	while (input[i])
	{
		while (input[i] == ' ' && !in_quotes)
			i++;
		if (!input[i])
			break ;
		if ((input[i] == '"' || input[i] == '\'') && !in_quotes)
		{
			in_quotes = true;
			quote_char = input[i++];
		}
		else if (in_quotes && input[i] == quote_char)
		{
			in_quotes = false;
			i++;
		}
		else if (!in_quotes && is_special_char(input[i]))
		{
			if (input[i] == '>' && input[i + 1] == '>')
				i++;
			count++;
			i++;
		}
		else
		{
			while (input[i] && ((in_quotes && input[i] != quote_char)
					|| (!in_quotes && !is_special_char(input[i]))))
				i++;
			count++;
		}
	}
	return (count);
}

static char	*extract_token(const char *input, int *i)
{
	int		start;
	int		len;
	bool	in_quotes;
	char	quote_char;
	char	*result;

	start = *i;
	len = 0;
	in_quotes = false;
	quote_char = 0;
	if (is_special_char(input[start]) && input[start] != ' ')
	{
		if (input[start] == '>' && input[start + 1] == '>')
		{
			*i += 2;
			return (ft_strdup(">>"));
		}
		char tmp[2] = {input[start], '\0'};
		*i += 1;
		return (ft_strdup(tmp));
	}
	while (input[*i])
	{
		if ((input[*i] == '"' || input[*i] == '\'') && !in_quotes)
		{
			in_quotes = true;
			quote_char = input[*i];
		}
		else if (in_quotes && input[*i] == quote_char)
			in_quotes = false;
		else if (!in_quotes && is_special_char(input[*i]))
			break ;
		len++;
		(*i)++;
	}
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	ft_strlcpy(result, &input[start], len + 1);
	return (result);
}

t_input	create_tokens(const char *input, char **env_copy)
{
	t_input	tok;
	int		i;
	int		j;
	int		token_index;

	i = 0;
	j = 0;
	token_index = 0;
	tok.env = env_copy;
	tok.token_count = count_tokens(input);
	tok.tokens = malloc(sizeof(t_token) * (tok.token_count + 1));
	if (!tok.tokens)
	{
		handle_error(MEMORY_ERROR, "");
		exit(EXIT_FAILURE);
	}
	while (j <= tok.token_count)
	{
		tok.tokens[j].token = NULL;
		tok.tokens[j].type = END;
		j++;
	}
	while (input[i] == ' ')
		i++;
	while (input[i] && token_index < tok.token_count)
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break ;
		tok.tokens[token_index].token = extract_token(input, &i);
		if (tok.tokens[token_index].token[0] != '\'')
			tok.tokens[token_index].token = replace_env(tok, tok.tokens[token_index].token);
		tok.tokens[token_index].token = ft_tokentrim(tok.tokens[token_index].token);
		tok.tokens[token_index].type
			= get_token_type(tok.tokens[token_index].token);
		token_index++;
	}
	return (tok);
}
