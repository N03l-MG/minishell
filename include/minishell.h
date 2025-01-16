/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:53:33 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/16 13:27:28 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libs.h"
# include "structs.h"

/*			FUNCTION PROTOTYPES		*/

/*		Token management		*/
//t_input	create_tokens(const char *input);
t_input	create_tokens(const char *input);
char	**token_copy(char **ptr, const char *s, size_t i);
size_t	spltnmb(const char *s, int totlen);
char	*ft_tokentrim(char *str);
void	free_tokens(t_token **ptr, int x);

/*	Shell functionality	*/
int		validate_input(t_input tokens, char **env);
void	handle_redir(char *input_file, char *output_file, t_ttype out_type);
void	execute_input(t_input tokens, char **env);

/*	Execution functions	*/
void	set_i(int *i, t_input *tokens);
char	**parse_command(t_input tokens, int cmd_start, int cmd_end);
void	setup_pipe(int *pipe_fds);
void	handle_child(t_data *data, int is_last, t_file *files, char **env);
void	handle_parent(t_data *data, int *prev_fd, pid_t pid, int is_last);

/*		Signals		*/
void	sig_sigint(int sig);

/*			Utils			*/
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strtok(char *str, const char *delim);
void	build_path(char *full_path, const char *path, const char *cmd);
char	*resolve_command_path(const char *command);
void	free_check_char(char *str);
bool	is_builtin(const char *cmd);

/*		Builtins		*/
int		execute_builtin(t_input tokens, char **env);
void	execute_builtin_piped(char **cmd, char **env);
void	change_directory(char *pth);
void	buildin_echo(t_input tok, int no_nl, int start_token);
void	print_working_dir(void);
void	print_envs(char **env);
void	export_variable(char **env, char *var);

/*	Error Handling		*/
int		handle_error(t_error error, char *current);
void	handle_mem_error(t_input *tokens);
void	free_allocated(t_data *data, t_input *tokens, t_error error);

#endif
