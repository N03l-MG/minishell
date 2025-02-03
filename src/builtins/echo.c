/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 10:38:52 by jgraf             #+#    #+#             */
/*   Updated: 2025/02/03 16:14:57 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_tokens(t_input tok, int start_idx)
{
	int	i;

	i = start_idx;
	while (i < tok.token_count)
	{
		if (tok.tokens[i].token)
		{
			printf("%s", tok.tokens[i].token);
			if (i < tok.token_count - 1)
				printf(" ");
		}
		i ++;
	}
}

void	builtin_echo(t_input *tok, int no_nl, int start_idx)
{
	if (tok->token_count == 1)
	{
		if (!no_nl)
			printf("\n");
		tok->last_status = 0;
		return ;
	}
	print_tokens(*tok, start_idx);
	if (!no_nl)
		printf("\n");
	tok->last_status = 0;
}
