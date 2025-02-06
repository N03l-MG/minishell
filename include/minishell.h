/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 07:53:33 by jgraf             #+#    #+#             */
/*   Updated: 2025/02/06 16:46:09 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libs.h"
# include "structs.h"

// ================== REDEFS ================= //

# define TEAL "\001\033[0;36m\002"
# define RED "\001\033[0;31m\002"
# define RESET "\001\033[0m\002"

// ================ PROTOTYPES =============== //

// ---------------- Builtins ---------------- //
void	change_dir(t_input *tok, char *path);
void	handle_cd(t_input *tokens);
void	builtin_echo(t_input *tok, int no_nl, int start_idx);
void	handle_echo(t_input *tokens, int start);
void	print_working_dir(t_input *tok);
void	print_envs(t_input *tok);
int		my_getenv_index(char **env, char *name);
char	**export_variable(char *var, t_input *tok);
char	**export_variable_sep(char *var, char *con, t_input tok);
void	handle_export(t_input *tokens);
char	**unset_variable(char *var, t_input *tok);
void	handle_unset(t_input *tokens);

// ------------- Builtins Utils ------------- //
bool	is_builtin(const char *cmd);
int		check_valid_export_unset(char *var_name);
void	print_sorted_env(char **env);
void	create_and_add_new(char *env, char *name, char *con);
char	*replace_and_add_new(char *repl, char *name, char *con, char *env);

// --------------- Environment -------------- //
char	**init_env(char **environ);
char	**add_envvar_pre(char **env, char *name, char *con);
char	**replace_envvar_pre(char **env, char *name, char *con);
char	**add_envvar(t_input tok, char *name, char *con);
char	**replace_envvar(t_input tok, char *name, char *con);
int		get_entry_number(char **environ);

// --------------- Tokenizing --------------- //
t_input	create_tokens(const char *input, char **env_copy, int last_status);
t_ttype	get_token_type(const char *str);
char	*ft_tokentrim(char *str);
char	*extract_token(const char *input, int *i);

// ---------------- Parsing ----------------- //
char	*my_getenv(char **env, char *name);
char	*replace_env(t_input token, char *tok);
bool	check_unclosed_quotes(t_input *token, const char *input);
char	*handle_specials(const char *input, int start, int *i);
int		validate_input(t_input *tokens);

// --------------- Execution ---------------- //
void	execute_input(t_input *tokens);
int		execute_builtin(t_input *tokens);
void	handle_child(t_data *data, int is_last, t_file *files, char **env);
void	handle_parent(t_data *data, int *prev_fd, pid_t pid, int is_last);

// ------------- Execution Utils ------------ //
void	set_i(int *i, t_input *tokens);
char	**parse_cmd(t_input tokens, int cmd_start, int cmd_end);
void	setup_pipe(int *pipe_fds);
char	*handle_heredoc(t_input *tok, char *delimiter);
void	get_redir(t_input tokens, int *cmd_start, int *cmd_end, t_file *files);
void	handle_redir(char *infile, char *outfile, t_ttype ttype, t_input *tok);

// ---------------- Signals ----------------- //
void	sig_sigint(int sig);
bool	get_and_reset_sigint(void);

// -------------- Random Utils -------------- //
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strtok(char *str, const char *delim);
char	*ft_strndup(const char *s, size_t n);
bool	is_special_char(char c);
int		count_tokens(const char *input);
void	build_path(char *full_path, const char *path, const char *cmd);
char	*resolve_command_path(char **env, const char *command);
void	free_check_char(char *str);

// ----------- Errors and Cleanup ----------- //
void	print_error(t_error error, char *context);
int		handle_error(t_error error, char *context, t_input *tok);
void	handle_fatal_error(t_error error, char *context, t_input *tokens);
void	free_env(char **env);
void	free_tokens(t_token **ptr, int x);
void	free_cmd_array(char **cmd);
void	cleanup_command(t_data *data);
void	clean_exit(t_input *tokens, t_data *data);

#endif
