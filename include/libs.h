/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:44:07 by nmonzon           #+#    #+#             */
/*   Updated: 2025/02/04 17:07:02 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBS_H
# define LIBS_H

// My libft with all of its functions plus my fprintf and get_next_line
# include "../libft/include/libft.h"

// Some C standard libraries
# include <stdio.h>
# include <signal.h>
# include <errno.h>
# include <string.h>

// Libraries Specific to minishell related utilities
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>

// Linux compatibility for waitpid (minishell was developed on MacOS and Ubuntu)
# ifdef __linux__
#  include <wait.h>
# endif

#endif
