/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:44:32 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/06 16:05:03 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*prompt;
	t_token	tokens;

	signal(SIGINT, SIG_IGN);
	while (true)
	{
		prompt = readline("minishell> ");
		if (prompt != NULL)
		{
			if (*prompt == '\0')
			{
				free(prompt);
				continue ;
			}
			if (ft_strcmp(prompt, "exit") == 0)
			{
				free(prompt);
				break ;
			}
			tokens = create_tokens(prompt);
			add_history(prompt);
			if (validate_input(tokens) == 0)
				printf("Excuting %s\n", prompt + 11);
			free(prompt);
			free(tokens.tokens);
		}
		else
			printf("Couldn't read from commandline!\n");
	}
	return (0);
}
