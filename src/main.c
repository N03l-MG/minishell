/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:44:32 by jgraf             #+#    #+#             */
/*   Updated: 2025/02/06 10:26:49 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/// @brief: Main shell runtime loop. Parses, validates and executes input.
/// @param: char **env_copy - runtime copy of the environment variables.
/// @param: t_input *tokens - struct containing all the tokens and global data.
static void	minishell(char **env_copy, t_input *tokens)
{
	char	*prompt;

	while (true)
	{
		prompt = readline(TEAL "MINISHELL❯ " RESET);
		if (prompt == NULL)
			break ;
		else
		{
			if (get_and_reset_sigint())
				tokens->last_status = 130;
			if (prompt[0] == '\0')
			{
				free(prompt);
				continue ;
			}
			*tokens = create_tokens(prompt, env_copy, tokens->last_status);
			add_history(prompt);
			free(prompt);
			if (validate_input(tokens) == 0)
				execute_input(tokens);
			free_tokens(&tokens->tokens, tokens->token_count - 1);
			env_copy = tokens->env;
		}
	}
}

int	main(int ac, char **av, char **env)
{
	t_input	tokens;
	char	**env_copy;

	(void)ac, (void)av;
	signal(SIGINT, sig_sigint);
	signal(SIGQUIT, SIG_IGN);
	env_copy = init_env(env);
	tokens.last_status = 0;
	minishell(env_copy, &tokens);
	free_env(tokens.env);
	return (tokens.last_status);
}
