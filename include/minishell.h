/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:53:33 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/21 15:45:27 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libs.h"
# include "structs.h"

# define TEAL "\033[0;36m"
# define RED "\033[0;31m"
# define RESET "\033[0m"

/*			FUNCTION PROTOTYPES		*/

char	**add_envvar_pre(char **env, char *name, char *con);
char	**replace_envvar_pre(char **env, char *name, char *con);
void	create_and_add_new(char *env, char *name, char *con);
char	*replace_and_add_new(char *repl, char *name, char *con, char *env);

/*		Token management		*/
t_input	create_tokens(const char *input, char **env_copy);
char	*ft_tokentrim(char *str);
char	*replace_env(t_input token, char *tok);
int		check_quote_closed(t_input *tokens);
void	free_tokens(t_token **ptr, int x);

/*	Shell functionality	*/
int		validate_input(t_input *tokens);
void	handle_redir(char *input_file, char *output_file, t_ttype out_type);
void	execute_input(t_input tokens);
char	**init_env(char **environ);
char	**add_envvar(t_input tok, char *name, char *con);
char	**replace_envvar(t_input tok, char *name, char *con);
char	*my_getenv(char **env, char *name);

/*	Execution functions	*/
void	set_i(int *i, t_input *tokens);
char	**parse_command(t_input tokens, int cmd_start, int cmd_end);
void	setup_pipe(int *pipe_fds);
void	handle_child(t_data *data, int is_last, t_file *files, char **env);
void	handle_parent(t_data *data, int *prev_fd, pid_t pid, int is_last, int *status);

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
int		execute_builtin(t_input *tokens);
void	execute_builtin_piped(char **cmd, char **env);
void	change_dir(t_input *tok, char *path);
void	buildin_echo(t_input tok, int no_nl, int start_token);
void	print_working_dir(t_input *tok);
void	print_envs(t_input *tok);
char	**export_variable(char *var, t_input tok);
char	**export_variable_sep(char *var, char *con, t_input tok);
char	**unset_variable(char *var, t_input tok);

/*	Error Handling		*/
int		handle_error(t_error error, char *current, t_input *tok);
void	handle_mem_error(t_input *tokens);
void	free_allocated(t_data *data, t_input *tokens, t_error error);
void	free_env(char **env);

#endif
