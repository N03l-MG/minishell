/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:04:09 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/27 11:04:10 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';' || c == ' ');
}

t_ttype	get_token_type(const char *str)
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

char	*handle_specials(const char *input, int start, int *i)
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

int	count_tokens(const char *input)
{
	int		count;
	int		i;
	char	*token;

	count = 0;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break ;
		token = extract_token(input, &i);
		if (token)
		{
			free(token);
			count++;
		}
	}
	return (count);
}
