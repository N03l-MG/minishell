/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:03:24 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/06 21:33:23 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	exit_atoi(char *str)
{
	int		i;
	int		sign;
	long	result;
	int		sign_count;

	i = 0;
	sign = 1;
	result = 0;
	sign_count = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i ++;
	while (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign = -1;
		sign_count ++;
	}
	if (sign_count > 1)
		return (255);
	while (str[i] != '\0' && ft_isdigit(str[i]))
		result = result * 10 + (str[i++] - '0');
	if (str[i] != '\0')
		return (255);
	return ((int)(result * sign));
}

void	clean_exit(t_input *tokens, t_data *data)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	free_heredoc_files();
	if (tokens->token_count >= 2)
	{
		if (tokens->tokens[1].token[0] == '\0')
			tokens->last_status = 255;
		else
			tokens->last_status = exit_atoi(tokens->tokens[1].token);
	}
	while (tokens->last_status < 0)
		tokens->last_status += 256;
	status = tokens->last_status;
	free_input(tokens);
	if (data != NULL)
		cleanup_command(data);
	exit(status);
}
