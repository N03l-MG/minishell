/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:53:33 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/27 17:05:07 by nmonzon          ###   ########.fr       */
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
int		get_entry_number(char **environ);

/*		Token management		*/
t_input	create_tokens(const char *input, char **env_copy, int last_status);
char	*ft_tokentrim(char *str);
char	*replace_env(t_input token, char *tok);
bool	check_unclosed_quotes(t_input *token, const char *input);
char	*extract_token(const char *input, int *i);
void	free_tokens(t_token **ptr, int x);

/*	Shell functionality	*/
int		validate_input(t_input *tokens);
void	handle_redir(char *input_file, char *output_file, t_ttype out_type);
void	execute_input(t_input *tokens);
char	**init_env(char **environ);
char	**add_envvar(t_input tok, char *name, char *con);
char	**replace_envvar(t_input tok, char *name, char *con);
char	*my_getenv(char **env, char *name);

/*	Execution functions	*/
void	set_i(int *i, t_input *tokens);
char	**parse_command(t_input tokens, int cmd_start, int cmd_end);
void	setup_pipe(int *pipe_fds);
void	handle_child(t_data *data, int is_last, t_file *files, char **env);
void	handle_parent(t_data *data, int *prev_fd, pid_t pid, int is_last);
char	*handle_heredoc(char *delimiter);
void	get_redir(t_input tokens, int *cmd_start, int *cmd_end, t_file *files);

/*		Signals		*/
void	sig_sigint(int sig);
bool	get_and_reset_sigint(void);

/*			Utils			*/
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strtok(char *str, const char *delim);
void	build_path(char *full_path, const char *path, const char *cmd);
char	*resolve_command_path(const char *command);
void	free_check_char(char *str);
bool	is_builtin(const char *cmd);
char	*ft_strndup(const char *s, size_t n);
bool	is_special_char(char c);
t_ttype	get_token_type(const char *str);
char	*handle_specials(const char *input, int start, int *i);
int		count_tokens(const char *input);

/*		Builtins		*/
int		execute_builtin(t_input *tokens);
void	execute_builtin_piped(char **cmd, char **env);
void	change_dir(t_input *tok, char *path);
void	builtin_echo(t_input tok, int no_nl);
void	print_working_dir(t_input *tok);
void	print_envs(t_input *tok);
void	print_sorted_env(char **env);
char	**export_variable(char *var, t_input tok);
char	**export_variable_sep(char *var, char *con, t_input tok);
char	**unset_variable(char *var, t_input tok);

/*	Error Handling		*/
void	print_error(t_error error, char *context);
int		handle_error(t_error error, char *context, t_input *tok);
void	handle_fatal_error(t_error error, char *context, t_input *tokens);
void	free_env(char **env);
void	clean_exit(int status, t_input *tokens);

#endif
