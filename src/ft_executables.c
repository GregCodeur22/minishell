/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executables.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:09:03 by garside           #+#    #+#             */
/*   Updated: 2025/04/30 17:10:25 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_executables(t_data *data)
{
	int		status;
	char	**cmd;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd("fork failed\n", 2), 127);
	if (pid == 0)
	{
		cmd = ft_get_cmd(data);
		execve(cmd[0], cmd, data->envp);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free_split(cmd);
		free_data(data);
		exit(127);
	}
	waitpid(pid, &status, 0);
	return ((status >> 8) & 0xFF);
}
