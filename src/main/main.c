/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:44:32 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/16 13:19:18 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*prompt;
	t_input	tokens;

	(void)ac, (void)av;
	signal(SIGINT, sig_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (true)
	{
		prompt = readline("minishell> ");
		if (prompt == NULL)
		{
			ft_fprintf(2, "exiting...\n");
			break ;
		}
		else
		{
			if (prompt[0] == '\0')
			{
				free(prompt);
				continue ;
			}
			tokens = create_tokens(prompt);
			add_history(prompt);
			if (validate_input(tokens, env) == 0)
				execute_input(tokens, env);
			free(prompt);
			free_tokens(&tokens.tokens, tokens.token_count - 1);
		}
	}
	return (EXIT_SUCCESS);
}
