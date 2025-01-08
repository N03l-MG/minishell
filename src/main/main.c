/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:44:32 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/07 16:28:12 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_specials(char *prompt)
{
	if (*prompt == '\0')
	{
		free(prompt);
		return (1);
	}
	if ((ft_strncmp(prompt, "exit", ft_strlen(prompt)) == 0))
	{
		free(prompt);
		return (2);
	}
	return (0);
}

int	main(void)
{
	char	*prompt;
	t_token	tokens;
	int		special_result;

	signal(SIGINT, SIG_IGN);
	while (true)
	{
		prompt = readline("minishell> ");
		if (prompt != NULL)
		{
			special_result = handle_specials(prompt);
			if (special_result == 1)
				continue ;
			else if (special_result == 2)
				break ;
			tokens = create_tokens(prompt);
			add_history(prompt);
			if (validate_input(tokens) == 0)
				execute_input(tokens);
			free(prompt);
			free_tokens(tokens.tokens, tokens.token_count);
		}
	}
	return (0);
}
