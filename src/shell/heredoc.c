#include "minishell.h"

static char	*create_heredoc_file(void)
{
	static int  heredoc_count = 0;
	char        *number_str;
	char        *filename;

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
		unlink(filename);  // Delete the temporary file
		free(filename);
	}
}

char	*handle_heredoc(char *delimiter)
{
	char    *line;
	char    *filename;
	int     fd;

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
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			cleanup_heredoc(filename, fd);
			return (NULL);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	return (filename);
}
