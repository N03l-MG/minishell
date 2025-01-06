/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 09:57:06 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/06 16:06:15 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_token_nmb(const char *input)
{
	int	i;
	int	nmb;
	int	totlen;

	i = 0;
	nmb = 0;
	totlen = ft_strlen(input);
	while (i < totlen)
	{
		while (i < totlen && input[i] == ' ')
			i ++;
		if (i < totlen && input[i] != ' ')
		{
			nmb ++;
			while (i < totlen && input[i] != ' ')
				i ++;
		}
	}
	return (nmb);
}

t_token	create_tokens(const char *input)
{
	t_token	tok;
	int		i;

	i = 0;
	tok.tokens = ft_tokensplit(input, ' ');
	tok.token_count = get_token_nmb(input);
	//printf("Has %i tokens\n", tok.token_count);
	while (tok.tokens[i])
	{
		//printf("%s\n", tok.tokens[i]);
		i ++;
	}
	return (tok);
}
