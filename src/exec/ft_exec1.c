/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:37:08 by garside           #+#    #+#             */
/*   Updated: 2025/06/06 15:41:40 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_cmd_path(t_data *data, char **cmd)
{
	return (find_cmd_path(cmd[0], data));
}

void	is_not_path(t_data *data, char **args, char *path)
{
	if (access(data->token->value, F_OK) == 0)
	{
		if (access(data->token->value, X_OK) == 0)
		{
				execve(path, args, data->envp);
		}
		permission_denied(args[0]);
			if (data->cmd_list)
		free_cmd_list(data);
		free_data(data);
		exit(126);
	}
	ft_putstr_fd(data->token->value, 2);
	ft_putstr_fd(": command not found\n", 2);
	if (data->cmd_list)
		free_cmd_list(data);
	free_data(data);
	exit(127);
}

int	exec_child_process(t_data *data, t_cmd *cmd, int prev_fd)
{
	char	**args;
	char	*path;

	reset_signals_child();
	signal(SIGPIPE, SIG_IGN);
	args = cmd->args;
	path = get_cmd_path(data, args);
	if (redirect_management(cmd, prev_fd) == -1)
		return (CODE_FAIL);
	if (cmd->saved_stdin != STDIN_FILENO)
		close(cmd->saved_stdin);
	if (cmd->saved_stdout != STDOUT_FILENO)
		close(cmd->saved_stdout);
	if (!path)
		is_not_path(data, args, path);
	execve(path, args, data->envp);
	ft_putstr_fd("execve failed\n", 2);
	if (data->cmd_list)
		free_cmd_list(data);
	if (data)
		free_data(data);
	free(path);
	exit(127);
}

int	ft_shell(t_data *data, t_cmd *cmd, int prev_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd("fork failed\n", 2), 1);
	if (pid == 0)
		exec_child_process(data, cmd, prev_fd);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_sigint);
	if (WIFSIGNALED(status))
	{
		data->last_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
	else
		data->last_status = WEXITSTATUS(status);
	return (data->last_status);
}

int	which_command(t_data *data, t_cmd *cmd, int prev_fd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(data));
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(data));
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(data, cmd, cmd->saved_stdin, cmd->saved_stdout));
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(data, cmd));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(data));
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(data));
	if (ft_strncmp(cmd->args[0], "./", 2) == 0)
		return (ft_executables(data, cmd, cmd->saved_stdin, cmd->saved_stdout));
	return (ft_shell(data, cmd, prev_fd));
}
