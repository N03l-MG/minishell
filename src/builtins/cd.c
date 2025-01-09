/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraf <jgraf@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 08:14:13 by jgraf             #+#    #+#             */
/*   Updated: 2025/01/08 08:14:14 by jgraf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_directory(char *path)
{
	if (path == NULL || path[0] == '\0')
		handle_error(FILE_NOT_FOUND, path);
	if (access(path, F_OK | X_OK) != 0)
		handle_error(FILE_NOT_FOUND, path);
	else
		chdir(path);
}
