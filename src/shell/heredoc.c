/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:24:57 by nmonzon           #+#    #+#             */
/*   Updated: 2025/01/24 09:40:16 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_heredoc_file(void)
{
	static int	heredoc_count = 0;
	char		*number_str;
	char		*filename;

	number_str = ft_itoa(heredoc_count++);
	if (!number_str)
		return (NULL);
	filename = ft_strjoin("/tmp/.heredoc_", number_str);
	free(number_str);
	if (!filename)
		return (NULL);
	return (filename);
}

static void	cleanup_heredoc(char *filename, int fd)
{
	if (fd != -1)
		close(fd);
	if (filename)
	{
		unlink(filename);
		free(filename);
	}
}

static int	write_heredoc_content(int fd, char *delimiter)
{
	char	*line;

	while (true)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			return (0);
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (1);
}

char	*handle_heredoc(char *delimiter)
{
	char	*filename;
	int		fd;

	if (!delimiter)
		return (NULL);
	filename = create_heredoc_file();
	if (!filename)
		return (NULL);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		cleanup_heredoc(filename, -1);
		return (NULL);
	}
	if (write_heredoc_content(fd, delimiter) != 0)
	{
		cleanup_heredoc(filename, fd);
		return (NULL);
	}
	close(fd);
	return (filename);
}
