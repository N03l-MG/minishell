#include "minishell.h"

void	free_tokens(t_token **tokens, int count)
{
	int	i;

	if (!tokens || !*tokens)
		return ;
	i = 0;
	while (i <= count)
	{
		if ((*tokens)[i].token)
		{
			free((*tokens)[i].token);
			(*tokens)[i].token = NULL;
		}
		i++;
	}
	free(*tokens);
	*tokens = NULL;
}

// static void	free_data(t_data *data)
// {
// 	if (!data)
// 		return ;
// 	if (data->cmd)
// 	{
// 		free(data->cmd);
// 		data->cmd = NULL;
// 	}
// 	if (data->full_path)
// 	{
// 		free(data->full_path);
// 		data->full_path = NULL;
// 	}
// }

static void	free_heredoc_files(void)
{
	char	*filename;
	char	*number_str;
	int		i;

	i = 0;
	while (i < 1000)
	{
		number_str = ft_itoa(i);
		filename = ft_strjoin("/tmp/.heredoc_", number_str);
		free(number_str);
		if (access(filename, F_OK) == 0)
			unlink(filename);
		free(filename);
		i++;
	}
}

static void	free_input(t_input *tokens)
{
	if (!tokens)
		return ;
	if (tokens->tokens)
	{
		free_tokens(&tokens->tokens, tokens->token_count);
		tokens->tokens = NULL;
	}
	if (tokens->env)
	{
		free_env(tokens->env);
		tokens->env = NULL;
	}
}

void	clean_exit(int status, t_input *tokens)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	free_input(tokens);
	free_heredoc_files();
	exit(status);
}
