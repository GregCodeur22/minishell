/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:27:48 by garside           #+#    #+#             */
/*   Updated: 2025/06/01 20:06:05 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	exec_child(t_data *data, t_cmd *cmd, int prev_fd)
{
	reset_signals_child();
	if (!cmd || !cmd->args || !cmd->args[0])
		handle_invalid_command(data, cmd, prev_fd);
	if (redirect_management(cmd, prev_fd) == 1)
		ft_exit_exec(1, data, cmd);
	if (is_builtin(cmd->args[0]))
		ft_exit_exec(run_builtin(data, cmd), data, cmd);
	if (cmd->args[0][0] == '.' || cmd->args[0][0] == '/')
		handle_direct_exec(data, cmd);
	if (cmd->path)
		handle_path_exec(data, cmd);
	error_message(cmd->args[0]);
	ft_exit_exec(127, data, cmd);
}

int	resolve_command_path(t_data *data, t_cmd *cmd)
{
	if (cmd->args && cmd->args[0] && !is_builtin(cmd->args[0])
		&& cmd->args[0][0] != '.' && cmd->args[0][0] != '/')
	{
		cmd->path = find_cmd_path(cmd->args[0], data);
		if (!cmd->path)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			return (127);
		}
	}
	return (0);
}

int	ft_process(t_data *data, t_cmd *cmd, int prev_fd)
{
	pid_t	pid;
	int		ret;

	ret = resolve_command_path(data, cmd);
	if (ret != 0)
		return (ret);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (CODE_FAIL);
	}
	if (pid == 0)
		exec_child(data, cmd, prev_fd);
	if (cmd->path)
	{
		free(cmd->path);
		cmd->path = NULL;
	}
	return (pid);
}
