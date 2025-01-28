/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 10:38:52 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/28 12:31:25 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_token_content(char *token)
{
	int	j;

	j = 0;
	while (token[j])
	{
		printf("%c", token[j]);
		j++;
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
			print_token_content(tok.tokens[i].token);
			if (i < tok.token_count - 1)
				printf(" ");
		}
		i++;
	}
}

void	builtin_echo(t_input *tok, int no_nl)
{
	int	start_idx;

	if (tok->token_count == 1)
	{
		if (!no_nl)
			printf("\n");
		tok->last_status = 0;
		return ;
	}
	if (no_nl)
		start_idx = 2;
	else
		start_idx = 1;
	print_tokens(*tok, start_idx);
	if (!no_nl)
		printf("\n");
	tok->last_status = 0;
}
