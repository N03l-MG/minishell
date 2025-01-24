/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:44:32 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/24 12:15:38 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_input tokens);

int	main(int ac, char **av, char **env)
{
	char	*prompt;
	t_input	tokens;
	char	**env_copy;
	//t_gc	gc;

	(void)ac, (void)av;
	//gc_init(&gc);
	signal(SIGINT, sig_sigint);
	signal(SIGQUIT, SIG_IGN);
	env_copy = init_env(env);
	while (true)
	{
		prompt = readline(TEAL "MINISHELL❯ " RESET);
		if (prompt == NULL)
		{
			ft_fprintf(2, "Exiting...\n");
			break ;
		}
		else
		{
			if (prompt[0] == '\0')
			{
				free(prompt);
				continue ;
			}
			tokens = create_tokens(prompt, env_copy);
			//print_tokens(tokens);
			add_history(prompt);
			if (validate_input(&tokens) == 0)
				execute_input(tokens);
			free(prompt);
			free_tokens(&tokens.tokens, tokens.token_count - 1);
			env_copy = tokens.env;
		}
	}
	free_env(tokens.env);
	//gc_collect(&gc);
	return (EXIT_SUCCESS);
}

void	print_tokens(t_input tokens)
{
	int i = 0;
	while(i < tokens.token_count)
	{
		printf("Token %d: %s\n", i, tokens.tokens[i].token);
		i++;
	}
}
