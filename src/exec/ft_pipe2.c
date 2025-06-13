/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:58:33 by garside           #+#    #+#             */
/*   Updated: 2025/06/13 13:58:05 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	safe_close(int fd)
{
	if (fd >= 0)
	{
		close(fd);
	}
}

int	redirect_management(t_cmd *cmd, int prev_fd)
{
	if (manag_infile(cmd, prev_fd) == 1)
	{
		safe_close(cmd->pipe_fd[PIPE_READ]);
		safe_close(cmd->pipe_fd[PIPE_WRITE]);
		return (1);
	}
	if (manag_outfile(cmd, cmd->pipe_fd) == 1)
	{
		safe_close(cmd->pipe_fd[PIPE_READ]);
		safe_close(cmd->pipe_fd[PIPE_WRITE]);
		return (1);
	}
	safe_close(cmd->pipe_fd[PIPE_READ]);
	safe_close(cmd->pipe_fd[PIPE_WRITE]);
	return (0);
}

void	exit_d(t_data *data)
{
	if (!data->input)
	{
		ft_printf("exit\n");
		free_cmd_list(data);
		free_data(data);
		exit(0);
	}
}

void	handle_failed_permission(t_data *data, t_cmd *cmd, int prev_fd)
{
	if (!cmd->path || access(cmd->path, F_OK) == -1)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		safe_close(cmd->pipe_fd[PIPE_READ]);
		safe_close(cmd->pipe_fd[PIPE_WRITE]);
		ft_exit_exec(127, data, cmd);
	}
	else if (access(cmd->path, X_OK) == -1)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": permission denied\n", 2);
		safe_close(cmd->pipe_fd[PIPE_WRITE]);
		safe_close(prev_fd);
		ft_exit_exec(126, data, cmd);
	}
}
