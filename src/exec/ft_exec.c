/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:09:23 by garside           #+#    #+#             */
/*   Updated: 2025/06/10 18:01:10 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_single_command(t_data *data, t_cmd *cmd, int prev_fd)
{
	if (cmd->next == NULL)
	{
		cmd->saved_stdin = dup(STDIN_FILENO);
		cmd->saved_stdout = dup(STDOUT_FILENO);
		if (cmd->saved_stdin < 0 || cmd->saved_stdout < 0)
			return (perror("dup"), CODE_FAIL);
		if (redirect_management(cmd, prev_fd) == 1)
		{
			safe_close(cmd->saved_stdin);
			safe_close(cmd->saved_stdout);
			return (CODE_FAIL);
		}
		data->last_status = which_command(data, cmd, prev_fd);
		dup2(cmd->saved_stdin, STDIN_FILENO);
		dup2(cmd->saved_stdout, STDOUT_FILENO);
		safe_close(cmd->saved_stdin);
		safe_close(cmd->saved_stdout);
		return (data->last_status);
	}
	return (0);
}

void	handle_useless_command(t_cmd *cmd, int *prev_fd)
{
	int	tmp_fd;

	if (!cmd->args)
	{
		if (cmd->outfile)
		{
			tmp_fd = last_outfile(cmd);
			if (tmp_fd != -1)
			{
				safe_close(tmp_fd);
				safe_close(*prev_fd);
			}
		}
		if (cmd->infile)
		{
			tmp_fd = last_infile(cmd);
			if (tmp_fd != -1)
			{
				safe_close(*prev_fd);
				safe_close(tmp_fd);
			}
		}
	}
}

int	wait_for_children(t_data *data, pid_t last_pid, int prev_fd)
{
	int		status;
	pid_t	wpid;
	wpid = wait(&status);
	while (wpid > 0)
	{
		safe_close(data->cmd_list->pipe_fd[PIPE_READ]);
		safe_close(data->cmd_list->pipe_fd[PIPE_WRITE]);
		safe_close(prev_fd);
		if (wpid == last_pid)
		{
			if (WIFSIGNALED(status))
				data->last_status = 128 + WTERMSIG(status);
			else
				data->last_status = WEXITSTATUS(status);
		}
		wpid = wait(&status);
	}
	return (data->last_status);
}

int	exec_line(t_data *data, t_cmd *cmd)
{
	int		prev_fd;
	pid_t	last_pid;

	prev_fd = -1;
	last_pid = -1;
	if (cmd->next == NULL)
		return (handle_single_command(data, cmd, prev_fd));
	while (cmd)
	{
		if (cmd->next != NULL && pipe(cmd->pipe_fd) == -1)
			return (perror("pipe error"), 1);


		handle_useless_command(cmd, &prev_fd);
		last_pid = ft_process(data, cmd, prev_fd);
		safe_close(cmd->pipe_fd[PIPE_WRITE]);

		if (cmd->next != NULL)
		{
			if (prev_fd != -1)
				safe_close(prev_fd);
			prev_fd = cmd->pipe_fd[PIPE_READ];
		}
		else
		{
			safe_close(cmd->pipe_fd[PIPE_READ]);
			if (prev_fd != -1)
				safe_close(prev_fd);
		}
		cmd = cmd->next;

	}
	return (wait_for_children(data, last_pid, prev_fd));
}
