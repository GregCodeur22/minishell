/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executables.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:09:03 by garside           #+#    #+#             */
/*   Updated: 2025/06/03 18:44:17 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_clean(t_data *data)
{
	ft_putstr_fd("Error: invalid command or arguments\n", 2);
	free_cmd_list(data);
	free_data(data);
	exit(1);
}

void	exit_clean2(t_data *data, t_cmd *cmd)
{
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	free_cmd_list(data);
	free_data(data);
	exit(127);
}

void	redire_exec(t_data *data, t_cmd *cmd, int input_fd, int output_fd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		exit_clean(data);
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

int	ft_executables(t_data *data, t_cmd *cmd, int input_fd, int output_fd)
{
	struct stat	stat_info;
	int			status;
	pid_t		pid;

	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd("fork failed\n", 2), 127);
	if (pid == 0)
	{
		redire_exec(data, cmd, input_fd, output_fd);
		if (stat(cmd->args[0], &stat_info) == 0)
		{
			if (S_ISDIR(stat_info.st_mode))
			{
				is_a_directory(cmd->args[0]);
				free_cmd_list(data);
				free_data(data);
				exit(126);
			}
		}
		execve(cmd->args[0], cmd->args, data->envp);
		exit_clean2(data, cmd);
	}
	waitpid(pid, &status, 0);
	return ((status >> 8) & 0xFF);
}
