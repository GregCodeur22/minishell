/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:35:24 by garside           #+#    #+#             */
/*   Updated: 2025/05/14 18:02:49 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int execute_pipeline(t_data *data)
{
	t_cmd  *cmd = data->cmd_list;
	int prev_fd  = -1;
	pid_t pid;
	int status;
	
	while (cmd)
	{
		if (cmd->next && pipe(cmd->pipe) == -1)
		{
			ft_putstr_fd("Error pipe", 2);
			return (1);
		}
		pid = fork();
		if (pid == -1)
		{
			ft_putstr_fd("errro fork \n", 2);
			return (1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1) {
				printf("dup2 2\n");
				dup2(cmd->pipe[0], STDIN_FILENO);}
			if (cmd->pipe[1] != -1)
				{printf("dup2 1\n");
				dup2(cmd->pipe[1], STDOUT_FILENO);}
			exit(which_command(data, cmd));
		}
		// if (prev_fd != -1)
		// 	close(prev_fd);
		// if (cmd->next)
		// 	close(cmd->pipe[1]);
		prev_fd = cmd->pipe[0];
		cmd = cmd->next;
	}

	pid_t wpid = 0;
	while (wpid != -1)
	{
		wpid = wait(&status);
	}
	return 0;
}

void exec_child(t_data *data, t_cmd *cmd, int prev_fd)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			ft_putstr_fd("redirection fail\n", 2);
			exit(1);
		}
		close(prev_fd);
	}
	if (cmd->pipe[1] != -1)
	{
		if (dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("redir output", 2);
			exit(1);
		}
		close(cmd->pipe[1]);
	}
	// if (cmd->infile)
	// {
	// 	int infile_fd = open(cmd->outfile, O_RDONLY);
	// 	if (infile_fd == -1)
	// 	{
	// 		ft_putstr_fd("open infile fail", 2);
	// 		exit (1);
	// 	}
	// 	if (dup2(infile_fd, 0) == -1)
	// 	{
	// 		ft_putstr_fd("redir infile fail", 2);
	// 		exit (1);
	// 	}
	// 	close (infile_fd);
	// }
	// if (cmd->outfile)
	// {
	// 	int flags = O_WRONLY | O_CREAT | O_TRUNC;
	// 	if (cmd->append)
	// 		flags = O_WRONLY | O_CREAT | O_APPEND;
		
	// 	int outfile_fd = open(cmd->outfile, flags, 0644);
	// 	if (outfile_fd == -1)
	// 	{
	// 		ft_putstr_fd("open outfile fail", 2);
	// 		exit (1);
	// 	}
	// 	if (dup2(outfile_fd, 1) == -1)
	// 	{
	// 		ft_putstr_fd("redir out fail", 2);
	// 		exit (1);	
	// 	}
	// 	close (outfile_fd);
	// }
		exit(which_command(data, cmd));

}
