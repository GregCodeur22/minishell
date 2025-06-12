/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:11:08 by abeaufil          #+#    #+#             */
/*   Updated: 2025/06/12 16:56:48 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	empty_line(const char *str)
{
	while (*str)
	{
		if (!ft_isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

int	resolve_command_path(t_data *data, t_cmd *cmd)
{
	if (cmd->args && cmd->args[0] && !empty_line(cmd->args[0])
		&& !is_builtin(cmd->args[0]) && cmd->args[0][0] != '.'
		&& cmd->args[0][0] != '/')
	{
		cmd->path = find_cmd_path(cmd->args[0], data);
		if (!cmd->path)
			return (127);
	}
	return (0);
}

static void	handle_failed_resolution(t_data *data, t_cmd *cmd, int prev_fd)
{
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(":command not found\n", 2);
	safe_close(cmd->pipe_fd[PIPE_READ]);
	safe_close(cmd->pipe_fd[PIPE_WRITE]);
	safe_close(prev_fd);
	ft_exit_exec(127, data, cmd);
}

int	ft_process(t_data *data, t_cmd *cmd, int prev_fd)
{
	pid_t	pid;
	int		ret;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), CODE_FAIL);
	if (pid == 0)
	{
		ret = resolve_command_path(data, cmd);
		if (ret != 0)
			handle_failed_resolution(data, cmd, prev_fd);
		exec_child(data, cmd, prev_fd);
	}
	if (cmd->path)
	{
		free(cmd->path);
		cmd->path = NULL;
	}
	return (pid);
}
