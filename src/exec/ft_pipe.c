/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:27:48 by garside           #+#    #+#             */
/*   Updated: 2025/06/11 19:08:45 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "echo") == 0)
		return (true);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd, "env") == 0)
		return (true);
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
}

int	run_builtin(t_data *data, t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(data));
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(data, cmd));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(data));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(data, cmd, STDIN_FILENO, STDOUT_FILENO));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(data));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(data));
	return (1);
}

static void	handle_invalid_args(t_data *data, t_cmd *cmd, int prev_fd)
{
	char	*trimmed;

	trimmed = ft_strtrim(cmd->args[0], " \t");
	if (!trimmed || trimmed[0] == '\0')
	{
		free(trimmed);
		ft_putstr_fd(":command not found\n", 2);
		safe_close(cmd->pipe_fd[PIPE_READ]);
		safe_close(cmd->pipe_fd[PIPE_WRITE]);
		safe_close(prev_fd);
		ft_exit_exec(127, data, cmd);
	}
	free(trimmed);
}

static void	close_all_fds(int prev_fd, int pipe_read, int pipe_write)
{
	if (prev_fd != -1)
		safe_close(prev_fd);
	safe_close(pipe_read);
	safe_close(pipe_write);
}

void	exec_child(t_data *data, t_cmd *cmd, int prev_fd)
{
	int	status;

	reset_signals_child();
	if (redirect_management(cmd, prev_fd) == 1)
		return (safe_close(prev_fd), ft_exit_exec(1, data, cmd));
	if (!cmd || !cmd->args || !cmd->args[0])
		handle_invalid_command(data, cmd, prev_fd);
	handle_invalid_args(data, cmd, prev_fd);
	close_all_fds(prev_fd, cmd->pipe_fd[PIPE_READ], cmd->pipe_fd[PIPE_WRITE]);
	if (is_builtin(cmd->args[0]))
	{
		status = run_builtin(data, cmd);
		close_all_fds(prev_fd, cmd->pipe_fd[PIPE_READ],
			cmd->pipe_fd[PIPE_WRITE]);
		ft_exit_exec(status, data, cmd);
	}
	if (cmd->args[0][0] == '.' || cmd->args[0][0] == '/')
		handle_direct_exec(data, cmd, prev_fd);
	if (cmd->path)
		handle_path_exec(data, cmd);
	error_message(cmd->args[0]);
	ft_exit_exec(127, data, cmd);
}
