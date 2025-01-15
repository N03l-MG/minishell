#include "minishell.h"

static bool is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';' || c == ' ');
}

static t_token_type get_token_type(const char *str)
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

static int count_tokens(const char *input)
{
	int count = 0;
	int i = 0;
	bool in_quotes = false;
	char quote_char = 0;

	while (input[i])
	{
		while (input[i] == ' ' && !in_quotes)
			i++;
		if (!input[i])
			break;
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
			while (input[i] && ((in_quotes && input[i] != quote_char) || 
				(!in_quotes && !is_special_char(input[i]))))
				i++;
			count++;
		}
	}
	return (count);
}

static char *extract_token(const char *input, int *i)
{
	int start = *i;
	int len = 0;
	bool in_quotes = false;
	char quote_char = 0;
	char *result;

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
			break;
		len++;
		(*i)++;
	}
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	ft_strlcpy(result, &input[start], len + 1);
	return (result);
}

t_input create_tokens(const char *input)
{
	t_input tok;
	int i = 0;
	int token_index = 0;

	tok.token_count = count_tokens(input);
	tok.tokens = malloc(sizeof(t_token) * (tok.token_count + 1));
	if (!tok.tokens)
	{
		handle_error(MEMORY_ERROR, "");
		exit(EXIT_FAILURE);
	}
	for (int j = 0; j <= tok.token_count; j++)
	{
		tok.tokens[j].token = NULL;
		tok.tokens[j].type = END;
		tok.tokens[j].is_string = false;
	}
	while (input[i] == ' ')
		i++;
	while (input[i] && token_index < tok.token_count)
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break;

		tok.tokens[token_index].token = extract_token(input, &i);
		tok.tokens[token_index].type = get_token_type(tok.tokens[token_index].token);
		tok.tokens[token_index].is_string = (tok.tokens[token_index].type == STRING);
		token_index++;
	}

	return (tok);
}
