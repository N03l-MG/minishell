/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:52:14 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/06 15:21:35 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

// ================== ENUMS ================== //

// Enum for the different token types
typedef enum e_ttype
{
	STRING,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	END
}	t_ttype;

// Enum used for error handling and lookup
typedef enum e_error
{
	ENV_NOT_FOUND,
	FILE_NOT_FOUND,
	COMMAND_NOT_FOUND,
	INVALID_INPUT,
	INVALID_FILE,
	PIPE_ERROR,
	FORK_ERROR,
	EXEC_ERROR,
	MEMORY_ERROR,
	PERMISSION_ERROR,
	SYNTAX_ERROR
}	t_error;

// ================= STRUCTS ================= //
// Some structs (usually labled as "storage" structs) are solely
// for contining variables. This is because of the 4 argument limit in the norm.

// Struct for each token conataining its type and actual string
typedef struct s_token
{
	t_ttype	type;
	char	*token;
	char	*raw_token;
}	t_token;

// Struct used for sotring the tokens for each input cycle and some global data
typedef struct s_input
{
	char	**env;
	t_token	*tokens;
	int		token_count;
	bool	quote_error;
	int		last_status;
}	t_input;

// Quote related storage struct
typedef struct s_quote
{
	int	x;
	int	y;
	int	dquote;
	int	squote;
}	t_quote;

// Execution related storage struct
typedef struct s_data
{
	int		prev_fd;
	int		pipe_fds[2];
	char	**cmd;
	char	*full_path;
	int		status;
	t_input	*tokens;
	int		cmd_start;
	pid_t	pids[1024];
	int		pid_count;
}	t_data;

// Struct for file redirection
typedef struct s_file
{
	char	*infile;
	char	*outfile;
	t_ttype	out_type;
}	t_file;

// Command parsing data storage struct
typedef struct s_parse_args
{
	int		cmd_start;
	int		cmd_end;
	t_input	tokens;
	char	**cmd;
	int		valid_tokens;
}	t_parse_args;

// Storage struct for process execution
typedef struct s_process_args
{
	t_data	*data;
	t_input	*tokens;
	int		cmd_start;
	int		cmd_end;
}	t_process_args;

#endif
