/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:03:47 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/04 14:24:43 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redir_type(t_token token, t_token next, t_file *files)
{
	if (token.type == REDIR_OUT || token.type == APPEND)
	{
		files->outfile = next.token;
		files->out_type = token.type;
	}
	else if (token.type == REDIR_IN)
		files->infile = next.token;
	else if (token.type == HEREDOC)
	{
		if (files->infile)
		{
			unlink(files->infile);
			free(files->infile);
		}
		files->infile = handle_heredoc(next.token);
	}
}

static void	init_files(t_file *files)
{
	files->infile = NULL;
	files->outfile = NULL;
	files->out_type = REDIR_OUT;
}

void	get_redir(t_input tok, int *cmd_start, int *cmd_end, t_file *files)
{
	int	i;

	i = *cmd_start;
	init_files(files);
	while (i < *cmd_end)
	{
		if (tok.tokens[i].type >= REDIR_IN && tok.tokens[i].type
			<= HEREDOC)
		{
			if (i + 1 >= *cmd_end)
				return (handle_error(INVALID_INPUT,
						tok.tokens[i].token, &tok), (void)0);
			redir_type(tok.tokens[i], tok.tokens[i + 1], files);
			if (tok.tokens[i].type == HEREDOC && !files->infile)
				return (handle_error(INVALID_INPUT,
						tok.tokens[i + 1].token, &tok), (void)0);
			tok.tokens[i].type = END;
			tok.tokens[i + 1].type = END;
			i += 2;
		}
		else
			i++;
	}
}

static void	handle_redir_out(char *out, t_input *tok, t_ttype out_type)
{
	int	fd;

	if (out_type == APPEND)
		fd = open(out, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(out, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		handle_error(INVALID_FILE, out, tok);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	handle_redir(char *in, char *out, t_ttype out_type, t_input *tok)
{
	int	fd;

	if (in)
	{
		fd = open(in, O_RDONLY);
		if (fd == -1)
		{
			handle_error(INVALID_FILE, in, tok);
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (out)
		handle_redir_out(out, tok, out_type);
}
