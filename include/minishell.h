/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:53:33 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/06 15:56:00 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_error
{
	ENV_NOT_FOUND,
	FILE_NOT_FOUND,
	COMMAND_NOT_FOUND,
	INVALID_INPUT,
	MEMORY
}	t_error;

/*			STRUCTS			*/
typedef struct s_token
{
	char	**tokens;
	int		token_count;
}	t_token;

/*			FUNCTIONS		*/
/*		Token creation		*/
t_token	create_tokens(const char *input);
char	**ft_tokensplit(const	char *s, char c);
/*		Token validation	*/
int		validate_input(t_token tokens);
int		handle_error(t_error error, char *current_token);
/*			Utils			*/
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strtok(char *str, const char *delim);

#endif
