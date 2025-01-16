/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 10:38:52 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/16 16:41:42 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	buildin_echo(t_input tok, int no_nl, int start_token)
{
	int	i;
	int	j;

	i = start_token + 1 + no_nl;
	while (i < tok.token_count)
	{
		j = 0;
		while (tok.tokens[i].token[j] != '\0')
		{
			printf("%c", tok.tokens[i].token[j]);
			j ++;
		}
		if (i < tok.token_count - 1)
			printf(" ");
		i ++;
	}
	if (!no_nl)
		printf("\n");
}
