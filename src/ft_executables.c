/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executables.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:09:03 by garside           #+#    #+#             */
/*   Updated: 2025/05/29 17:07:52 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	dup_close(int oldfd, int newfd)
{
	dup2(oldfd, newfd);
	close(oldfd);
}

static void	exit_error(t_data *data, const char *msg, int code)
{
	ft_putstr_fd((char *)msg, 2);
	if (code == 127)
		ft_putstr_fd(": No such file or directory\n", 2);
	free_cmd_list(data);
	free_data(data);
	exit(code);
}

static void	child_exec(t_data *data, t_cmd *cmd, int in_fd, int out_fd)
{
	struct stat	info;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit_error(data, "Error: invalid command or arguments\n", 1);
	if (in_fd != STDIN_FILENO)
		dup_close(in_fd, STDIN_FILENO);
	if (out_fd != STDOUT_FILENO)
		dup_close(out_fd, STDOUT_FILENO);
	if (stat(cmd->args[0], &info) == 0 && S_ISDIR(info.st_mode))
	{
		is_a_directory(cmd->args[0]);
		exit_error(data, "", 126);
	}
	execve(cmd->args[0], cmd->args, data->envp);
	exit_error(data, cmd->args[0], 127);
}

int	ft_executables(t_data *data, t_cmd *cmd, int input_fd, int output_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (ft_putstr_fd("fork failed\n", 2), 127);
	if (pid == 0)
		child_exec(data, cmd, input_fd, output_fd);
	waitpid(pid, &status, 0);
	return ((status >> 8) & 0xFF);
}
