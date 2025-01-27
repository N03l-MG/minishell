/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 08:14:13 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/27 15:43:59 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_path_access(char *path, t_input *tok)
{
	if (!path || path[0] == '\0')
	{
		handle_error(INVALID_INPUT, "cd: path required", tok);
		return (0);
	}
	if (access(path, F_OK) != 0)
	{
		handle_error(INVALID_FILE, path, tok);
		return (0);
	}
	if (access(path, X_OK) != 0)
	{
		handle_error(PERMISSION_ERROR, path, tok);
		return (0);
	}
	return (1);
}

static void	update_pwd(t_input *tok)
{
	char	pwd[1024];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
	{
		tok->env = export_variable_sep("PWD", pwd, *tok);
		tok->last_status = 0;
	}
}

void	change_dir(t_input *tok, char *path)
{
	if (!check_path_access(path, tok))
		return ;
	if (chdir(path) != 0)
	{
		handle_error(EXEC_ERROR, path, tok);
		return ;
	}
	update_pwd(tok);
}
