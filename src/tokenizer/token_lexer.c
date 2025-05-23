/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:14:17 by kixik             #+#    #+#             */
/*   Updated: 2025/02/06 12:46:13 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_quoted(const char *input, int *i, bool *in_quotes,
						char *quote_char)
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
		else if (!*in_quotes
			&& (is_special_char(input[*i]) || input[*i] == '\t'))
			break ;
		len++;
		(*i)++;
	}
	return (ft_strndup(&input[start], len));
}

char	*extract_token(const char *input, int *i)
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

static void	process_token(t_input *tok, int *index, const char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	if (!input[*i])
		return ;
	tok->tokens[*index].token = extract_token(input, i);
	tok->tokens[*index].raw_token = ft_strdup(tok->tokens[*index].token);
	tok->tokens[*index].type = get_token_type(tok->tokens[*index].token);
	tok->tokens[*index].token = replace_env(*tok, tok->tokens[*index].token);
	tok->tokens[*index].token = ft_tokentrim(tok->tokens[*index].token);
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

t_input	create_tokens(const char *input, char **env_copy, int last_status)
{
	t_input	tok;
	int		i;
	int		index;

	tok.env = env_copy;
	tok.last_status = last_status;
	tok.token_count = count_tokens(input);
	tok.quote_error = check_unclosed_quotes(&tok, input);
	tok.tokens = malloc(sizeof(t_token) * (tok.token_count + 1));
	if (!tok.tokens)
		handle_fatal_error(MEMORY_ERROR, NULL, &tok);
	init_tokens(&tok, tok.token_count);
	i = 0;
	index = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	while (input[i] && index < tok.token_count)
		process_token(&tok, &index, input, &i);
	return (tok);
}
