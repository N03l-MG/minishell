/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 08:14:13 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/20 16:03:23 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_dir(t_input *tok, char *path)
{
	char	pwd[1024];

	if (!path || path[0] == '\0')
	{
		handle_error(INVALID_INPUT, "cd: path required", tok);
		return ;
	}
	if (access(path, F_OK) != 0)
	{
		handle_error(INVALID_FILE, path, tok);
		return ;
	}
	if (access(path, X_OK) != 0)
	{
		handle_error(PERMISSION_ERROR, path, tok);
		return ;
	}
	if (chdir(path) != 0)
	{
		handle_error(EXEC_ERROR, path, tok);
		return ;
	}
	if (getcwd(pwd, sizeof(pwd)) != NULL)
	{
		tok->env = export_variable_sep("PWD", pwd, *tok);
		tok->env = export_variable_sep("LASTSTATUS", "0", *tok);
	}
}
