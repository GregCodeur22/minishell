/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 02:51:50 by garside           #+#    #+#             */
/*   Updated: 2025/06/05 14:16:17 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	command_not_found(char *cmd)
{
	ft_putstr_fd("Minishell:~$ ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": Command not found\n", 2);
}

void	no_such_file_or_directory(char *cmd)
{
	ft_putstr_fd("Minishell:~$ ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

void	permission_denied(char *file)
{
	ft_putstr_fd("Minishell:~$ ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": Permission denied\n", 2);
}

void	error_message(char *str)
{
	ft_putstr_fd("Minishell:~$ ", 2);
	perror(str);
}

void	is_a_directory(char *str)
{
	ft_putstr_fd("Minishell:~$ ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": Is a directory\n", 2);
}
