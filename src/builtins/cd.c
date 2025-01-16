/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 08:14:13 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/16 16:52:30 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_dir(t_input *tok, char *path)
{
	char	pwd[1024];

	if (path == NULL || path[0] == '\0')
		handle_error(FILE_NOT_FOUND, path);
	if (access(path, F_OK | X_OK) != 0)
		handle_error(FILE_NOT_FOUND, path);
	else
	{
		chdir(path);
		if (getcwd(pwd, sizeof(pwd)) != NULL)
			tok->env = export_variable_sep(tok->env, "PWD", pwd, *tok);
	}
}
