/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:52:14 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/16 16:28:06 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

/*			ENUM			*/

typedef enum e_ttype
{
	STRING,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	SEMICOLON,
	END
}	t_ttype;

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
	NOTANERROR
}	t_error;

/*			STRUCTS			*/

typedef struct s_token
{
	t_ttype	type;
	bool	is_string;
	char	*token;
}	t_token;

typedef struct s_input
{
	char	**env;
	t_token	*tokens;
	int		token_count;
}	t_input;

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

typedef struct s_file
{
	char	*infile;
	char	*outfile;
	t_ttype	out_type;
}	t_file;

#endif
