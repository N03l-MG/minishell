/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:53:33 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/07 16:23:42 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

/*			ENUM			*/

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

typedef struct s_quote
{
	int	x;
	int	y;
	int	dquote;
	int	squote;
}	t_quote;

typedef struct s_execvars
{
	int		pipe_fds[2];
	char	*full_path;
	char	**cmd;
	pid_t	pid;
}	t_execvars;

/*			FUNCTIONS		*/

/*	Token management	*/
t_token	create_tokens(const char *input);
char	**token_copy(char **ptr, const char *s, size_t i);
size_t	spltnmb(const char *s, int totlen);
char	*ft_tokentrim(char const *s1, char const *set);
void	free_tokens(char **ptr, int x);

/*	Shell functionality	*/
int		validate_input(t_token tokens);
void	execute_input(t_token tokens);

/*		Utils			*/
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strtok(char *str, const char *delim);
void	build_path(char *full_path, const char *path, const char *cmd);

/*	Error Handling		*/
int		handle_error(t_error error, char *current_token);

#endif
