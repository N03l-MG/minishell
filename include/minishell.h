/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:53:33 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/09 17:09:03 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

/*			ENUM			*/

typedef enum e_error
{
	ENV_NOT_FOUND,
	FILE_NOT_FOUND,
	COMMAND_NOT_FOUND,
	INVALID_INPUT,
	INVALID_FILE,
	PIPE,
	FORK,
	EXEC,
	MEMORY,
	NOTANERROR
}	t_error;

/*			STRUCTS			*/

typedef struct s_token
{
	char	**tokens;
	bool	*is_string;
	int		token_count;
}	t_token;

typedef struct s_quote
{
	int	x;
	int	y;
	int	dquote;
	int	squote;
}	t_quote;

typedef struct s_data
{
	int		prev_fd;
	int		pipe_fds[2];
	char	**cmd;
	char	*full_path;
}	t_data;

/*			FUNCTIONS		*/

/*		Token management		*/
t_token	create_tokens(const char *input);
char	**token_copy(char **ptr, const char *s, size_t i);
size_t	spltnmb(const char *s, int totlen);
char	*ft_tokentrim(char *str);
void	free_tokens(char **ptr, int x);

/*	Shell functionality	*/
int		validate_input(t_token tokens);
void	handle_redirections(char *input_file, char *output_file);
void	execute_input(t_token tokens);

/*	Execution functions	*/
void	set_i(int *i, t_token *tokens);
char	**parse_command(t_token tokens, int cmd_start, int cmd_end);
void	setup_pipe(int *pipe_fds);
void	handle_child(t_data *data, int is_last, char *infile, char *outfile);
void	handle_parent(t_data *data, int *prev_fd, pid_t pid, int is_last);

/*		Signals		*/
void	sig_sigint(int sig);

/*			Utils			*/
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strtok(char *str, const char *delim);
void	build_path(char *full_path, const char *path, const char *cmd);
char	*resolve_command_path(const char *command);
void	free_check_char(char *str);

/*		Buildins		*/
int		execute_buildin(t_token tokens, char **env);
void	change_directory(char *pth);
void	buildin_echo(t_token tok, int no_nl, int start_token);
void	print_working_dir(void);
void	print_envs(char **env);
void	export_variable(char **env, char *var);

/*	Error Handling		*/
int		handle_error(t_error error, char *current);
void	handle_mem_error(t_token *tokens);
void	free_allocated(t_data *data, t_token *tokens, t_error error);

#endif
