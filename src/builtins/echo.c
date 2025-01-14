/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 10:38:52 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/13 15:44:51 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_from_string(char *str, int *i)
{
	char	*variable;
	int		j;
	int		len;

	j = 0;
	len = 0;
	(*i)++;
	while (ft_isalpha(str[*i]) && str[*i + len] != '\0')
		len ++;
	variable = malloc(len + 1);
	if (variable == NULL)
	{
		handle_error(MEMORY_ERROR, "");
		return (NULL);
	}
	while (ft_isalpha(str[*i]) && str[*i] != '\0')
	{
		variable[j] = str[*i];
		(*i)++;
		j ++;
	}
	variable[j] = '\0';
	return (variable);
}

// static void	print_env(char *str, int *i)
// {
// 	char	*variable;

// 	variable = get_var_from_string(str, i);
// 	if (getenv(variable) != NULL)
// 	{
// 		printf("%s", getenv(variable));
// 		free(variable);
// 	}
// 	else
// 		(*i)--;
// }

void	buildin_echo(t_input tok, int no_nl, int start_token)
{
	int	i;
	int	j;

	i = start_token + 1 + no_nl;
	while (i < tok.token_count)
	{
		j = 0;
		while (tok.tokens[i].token[j] != '\0')
		{
			printf("%c", tok.tokens[i].token[j]);
			j ++;
		}
		if (i < tok.token_count - 1)
			printf(" ");
		i ++;
	}
	if (!no_nl)
		printf("\n");
}
