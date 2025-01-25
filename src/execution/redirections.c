#include "minishell.h"

static void	handle_redir_type(t_token token, t_token next_token, t_file *files)
{
	if (token.type == REDIR_OUT || token.type == REDIR_APPEND)
	{
		files->outfile = next_token.token;
		files->out_type = token.type;
	}
	else if (token.type == REDIR_IN)
		files->infile = next_token.token;
	else if (token.type == REDIR_HEREDOC)
	{
		if (files->infile)
		{
			unlink(files->infile);
			free(files->infile);
		}
		files->infile = handle_heredoc(next_token.token);
	}
}

static void	init_files(t_file *files)
{
	files->infile = NULL;
	files->outfile = NULL;
	files->out_type = REDIR_OUT;
}

void	get_redir(t_input tokens, int *cmd_start, int *cmd_end, t_file *files)
{
	int	i;

	i = *cmd_start;
	init_files(files);
	while (i < *cmd_end)
	{
		if (tokens.tokens[i].type >= REDIR_IN
			&& tokens.tokens[i].type <= REDIR_HEREDOC)
		{
			if (i + 1 >= *cmd_end)
			{
				handle_error(INVALID_INPUT, tokens.tokens[i].token, &tokens);
				return ;
			}
			handle_redir_type(tokens.tokens[i], tokens.tokens[i + 1], files);
			if (tokens.tokens[i].type == REDIR_HEREDOC && !files->infile)
			{
				handle_error(INVALID_INPUT, tokens.tokens[i + 1].token,
					&tokens);
				return ;
			}
			tokens.tokens[i].type = END;
			tokens.tokens[i + 1].type = END;
			i += 2;
		}
		else
			i++;
	}
}

void	handle_redir(char *input_file, char *output_file, t_ttype out_type)
{
	int	fd;
	int	flags;

	if (input_file)
	{
		fd = open(input_file, O_RDONLY);
		if (fd == -1)
			handle_error(INVALID_FILE, input_file, NULL);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (output_file)
	{
		flags = O_CREAT | O_WRONLY;
		if (out_type == REDIR_APPEND)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(output_file, flags, 0644);
		if (fd == -1)
			handle_error(INVALID_FILE, output_file, NULL);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
