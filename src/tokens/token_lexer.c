/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:14:17 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/24 12:13:25 by nmonzon          ###   ########.fr       */
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
	if (!strcmp(str, "<<"))
		return (REDIR_HEREDOC);
	return (STRING);
}

static char	*handle_specials(const char *input, int start, int *i)
{
	char	*tmp;

	if (is_special_char(input[start]) && input[start] != ' ')
	{
		if (input[start] == '>' && input[start + 1] == '>')
		{
			*i = start + 2;
			return (ft_strdup(">>"));
		}
		if (input[start] == '<' && input[start + 1] == '<')
		{
			*i = start + 2;
			return (ft_strdup("<<"));
		}
		tmp = (char *)malloc(2);
		tmp[0] = input[start];
		tmp[1] = '\0';
		*i = start + 1;
		return (tmp);
	}
	return (NULL);
}

static char	*extract_quoted(const char *input, int *i, bool *in_quotes, char *quote_char)
{
	int	start;
	int	len;

	start = *i;
	len = 0;
	while (input[*i])
	{
		if ((input[*i] == '"' || input[*i] == '\'') && !*in_quotes)
		{
			*in_quotes = true;
			*quote_char = input[*i];
		}
		else if (*in_quotes && input[*i] == *quote_char)
			*in_quotes = false;
		else if (!*in_quotes && is_special_char(input[*i]))
			break ;
		len++;
		(*i)++;
	}
	return (ft_strndup(&input[start], len));
}

static char	*extract_token(const char *input, int *i)
{
	bool	in_quotes;
	char	quote_char;
	char	*special_token;

	special_token = handle_specials(input, *i, i);
	if (special_token)
		return (special_token);
	in_quotes = false;
	quote_char = 0;
	return (extract_quoted(input, i, &in_quotes, &quote_char));
}

static int	count_tokens(const char *input)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (extract_token(input, &i) != NULL)
			count++;
		i++;
	}
	return (count);
}

static void	process_token(t_input *tok, int *index, const char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
	if (!input[*i])
		return ;
	tok->tokens[*index].token = extract_token(input, i);
	if (tok->tokens[*index].token[0] != '\'')
		tok->tokens[*index].token
			= replace_env(*tok, tok->tokens[*index].token);
	tok->tokens[*index].token
		= ft_tokentrim(tok->tokens[*index].token);
	tok->tokens[*index].type
		= get_token_type(tok->tokens[*index].token);
	(*index)++;
}

static void	init_tokens(t_input *tok, int count)
{
	int	j;

	j = 0;
	while (j <= count)
	{
		tok->tokens[j].token = NULL;
		tok->tokens[j].type = END;
		j++;
	}
}

t_input	create_tokens(const char *input, char **env_copy)
{
	t_input	tok;
	int		i;
	int		index;

	tok.env = env_copy;
	tok.token_count = count_tokens(input);
	tok.tokens = malloc(sizeof(t_token) * (tok.token_count + 1));
	if (!tok.tokens)
	{
		handle_mem_error(&tok);
		exit(EXIT_FAILURE);
	}
	init_tokens(&tok, tok.token_count);
	i = 0;
	index = 0;
	while (input[i] == ' ')
		i++;
	while (input[i] && index < tok.token_count)
		process_token(&tok, &index, input, &i);
	return (tok);
}
