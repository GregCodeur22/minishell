/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:50:01 by garside           #+#    #+#             */
/*   Updated: 2025/06/06 17:18:49 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit_exec(int code, t_data *data, t_cmd *cmd)
{
	if (cmd && cmd->path)
		free(cmd->path);
	if (data)
	{
		free_cmd_list(data);
		free_data(data);
	}
	exit(code);
}

int	open_infile(char *str)
{
	int	fd;

	fd = open(str, O_RDONLY);
	if (fd == -1)
	{
		if (access(str, F_OK) == -1)
			no_such_file_or_directory(str);
		else if (access(str, R_OK) == -1)
			permission_denied(str);
		else
			error_message(str);
	}
	return (fd);
}

void	handle_direct_exec(t_data *data, t_cmd *cmd, int prev_fd)
{
	if (access(cmd->args[0], F_OK) == -1)
		no_such_file_or_directory(cmd->args[0]);
	else if (access(cmd->args[0], X_OK) == -1)
		permission_denied(cmd->args[0]);
	else
		execve(cmd->args[0], cmd->args, data->envp);
	safe_close(prev_fd);
	safe_close(cmd->pipe_fd[PIPE_READ]);
	safe_close(cmd->pipe_fd[PIPE_WRITE]);
	safe_close(prev_fd);
	ft_exit_exec(126, data, cmd);
}

void	handle_path_exec(t_data *data, t_cmd *cmd)
{
	if (access(cmd->path, X_OK) == -1)
		permission_denied(cmd->path);
	else
		execve(cmd->path, cmd->args, data->envp);
	error_message(cmd->args[0]);
	ft_exit_exec(126, data, cmd);
}

void	handle_invalid_command(t_data *data, t_cmd *cmd, int prev_fd)
{
	safe_close(cmd->pipe_fd[PIPE_READ]);
	safe_close(cmd->pipe_fd[PIPE_WRITE]);
	safe_close(prev_fd);
	ft_exit_exec(0, data, cmd);
}
