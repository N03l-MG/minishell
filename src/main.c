/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:44:32 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/27 17:02:23 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	print_tokens(t_input tokens)
// {
// 	int	i;

// 	i = 0;
// 	while (i < tokens.token_count)
// 	{
// 		printf("Token %d: %s\n", i, tokens.tokens[i].token);
// 		i++;
// 	}
// }

int	main(int ac, char **av, char **env)
{
	char	*prompt;
	t_input	tokens;
	char	**env_copy;

	(void)ac, (void)av;
	signal(SIGINT, sig_sigint);
	signal(SIGQUIT, SIG_IGN);
	env_copy = init_env(env);
	tokens.last_status = 0;
	while (true)
	{
		prompt = readline(TEAL "MINISHELL❯ " RESET);
		if (prompt == NULL)
			break ;
		else
		{
			if (prompt[0] == '\0')
			{
				free(prompt);
				continue ;
			}
			if (get_and_reset_sigint())
			{
				tokens.last_status = 130;
				free(prompt);
				continue ;
			}
			tokens = create_tokens(prompt, env_copy, tokens.last_status);
			add_history(prompt);
			if (validate_input(&tokens) == 0)
				execute_input(&tokens);
			free(prompt);
			free_tokens(&tokens.tokens, tokens.token_count - 1);
			env_copy = tokens.env;
		}
	}
	free_env(tokens.env);
	return (EXIT_SUCCESS);
}
