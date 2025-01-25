/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 10:38:52 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/20 16:05:12 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_last_status(char **env)
{
	char	*status;

	status = my_getenv(env, "LASTSTATUS");
	printf("%s", status);
}

static void	print_token_content(char *token, char **env)
{
	int	j;

	j = 0;
	while (token[j])
	{
		if (token[j] == '$' && token[j + 1] == '?')
		{
			print_last_status(env);
			j += 2;
		}
		else
		{
			printf("%c", token[j]);
			j++;
		}
	}
}

static void	print_tokens(t_input tok, int start_idx)
{
	int	i;

	i = start_idx;
	while (i < tok.token_count)
	{
		if (tok.tokens[i].token)
		{
			print_token_content(tok.tokens[i].token, tok.env);
			if (i < tok.token_count - 1)
				printf(" ");
		}
		i++;
	}
}

void	builtin_echo(t_input tok, int no_nl)
{
	int	start_idx;

	if (tok.token_count == 1)
	{
		if (!no_nl)
			printf("\n");
		tok.env = export_variable_sep("LASTSTATUS", "0", tok);
		return ;
	}
	if (no_nl)
		start_idx = 2;
	else
		start_idx = 1;
	print_tokens(tok, start_idx);
	if (!no_nl)
		printf("\n");
	tok.env = export_variable_sep("LASTSTATUS", "0", tok);
}
