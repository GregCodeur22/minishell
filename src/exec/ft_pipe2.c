/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:58:33 by garside           #+#    #+#             */
/*   Updated: 2025/06/06 18:16:13 by garside          ###   ########.fr       */
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

// int	redirect_management(t_cmd *cmd, int *pipe_fd, int prev_fd)
// {
// 	if (management_infile(cmd, prev_fd) == CODE_FAIL)
// 		return (close(pipe_fd[0]), close(pipe_fd[1]), CODE_FAIL);
// 	else if (management_outfile(cmd, pipe_fd) == CODE_FAIL)
// 		return (close(pipe_fd[0]), close(pipe_fd[1]), CODE_FAIL);
// 	return (close(pipe_fd[0]), close(pipe_fd[1]), CODE_SUCCESS);
// }

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
