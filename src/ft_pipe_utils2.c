/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:40:50 by abeaufil          #+#    #+#             */
/*   Updated: 2025/05/29 17:36:42 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	last_outfile(t_cmd *cmd)
{
	int		fd;
	int		prev_fd;
	t_redir	*outfile;

	outfile = cmd->outfile;
	prev_fd = -1;
	while (outfile)
	{
		fd = open_outfile(outfile->file, outfile->type);
		if (fd == -1)
		{
			if (prev_fd != -1)
				safe_close(prev_fd);
			return (-1);
		}
		if (prev_fd != -1)
			safe_close(prev_fd);
		prev_fd = fd;
		outfile = outfile->next;
	}
	return (fd);
}

int	manag_outfile(t_cmd *cmd, int *pipe_fd)
{
	int	out_fd;

	if (cmd->outfile == NULL && cmd->next == NULL)
		return (0);
	if (cmd->outfile == NULL)
	{
		if (pipe_fd[PIPE_WRITE] >= 0)
		{
			dup2(pipe_fd[PIPE_WRITE], STDOUT_FILENO);
			safe_close(pipe_fd[PIPE_WRITE]);
			safe_close(pipe_fd[PIPE_READ]);
		}
		return (0);
	}
	out_fd = last_outfile(cmd);
	if (out_fd == -1)
	{
		if (pipe_fd[PIPE_WRITE] >= 0)
			dup2(pipe_fd[PIPE_WRITE], STDOUT_FILENO);
		return (1);
	}
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
	return (0);
}
