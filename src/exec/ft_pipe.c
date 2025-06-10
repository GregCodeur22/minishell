/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:27:48 by garside           #+#    #+#             */
/*   Updated: 2025/06/10 17:58:34 by garside          ###   ########.fr       */
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

void	exec_child(t_data *data, t_cmd *cmd, int prev_fd)
{
	char	*trimmed;
	// int dup_prev_fd = -1;

	reset_signals_child();

	// if (prev_fd != 1)
	// {
	// 	dup_prev_fd = dup(prev_fd);
	// 	if (dup_prev_fd == -1)
	// 		{
	// 			ft_exit_exec(1, data, cmd);
	// 		}
	// }
	if (redirect_management(cmd, prev_fd) == 1)
	{
		safe_close(prev_fd);
		ft_exit_exec(1, data, cmd);
	}
	
	if (!cmd || !cmd->args || !cmd->args[0])
		handle_invalid_command(data, cmd, prev_fd);
	trimmed = ft_strtrim(cmd->args[0], " \t");
	if (!trimmed || trimmed[0] == '\0')
	{
		free(trimmed);
		ft_putstr_fd(":command not found c toiiiiiiii   \n", 2);
		safe_close(cmd->pipe_fd[PIPE_READ]);
		safe_close(cmd->pipe_fd[PIPE_WRITE]);
		safe_close(prev_fd);
		ft_exit_exec(127, data, cmd);
	}
	free(trimmed);

	if (prev_fd != -1)
		safe_close(prev_fd);
	safe_close(cmd->pipe_fd[PIPE_READ]);
	safe_close(cmd->pipe_fd[PIPE_WRITE]);
	if (is_builtin(cmd->args[0]))
	{
		int status = run_builtin(data, cmd);
		safe_close(cmd->pipe_fd[PIPE_READ]);
		safe_close(cmd->pipe_fd[PIPE_WRITE]);
		safe_close(prev_fd);
		ft_exit_exec(status, data, cmd);
	}
	if (cmd->args[0][0] == '.' || cmd->args[0][0] == '/')
		handle_direct_exec(data, cmd, prev_fd);

	if (cmd->path)
		handle_path_exec(data, cmd);
	error_message(cmd->args[0]);
	ft_exit_exec(127, data, cmd);
}


int empty_line(const char *str)
{
    while (*str)
    {
        if (!isspace((unsigned char)*str))
            return (0);
        str++;
    }
    return (1);
}

int resolve_command_path(t_data *data, t_cmd *cmd)
{
	if (cmd->args && cmd->args[0]
		&& !empty_line(cmd->args[0])
		&& !is_builtin(cmd->args[0])
		&& cmd->args[0][0] != '.'
		&& cmd->args[0][0] != '/')
	{
			cmd->path = find_cmd_path(cmd->args[0], data);
			if (!cmd->path)
					return (127);
	}
	return (0);
}



int ft_process(t_data *data, t_cmd *cmd, int prev_fd)
{
    pid_t   pid;
    int     ret;
		
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (CODE_FAIL);
    }
    if (pid == 0)
    {
        ret = resolve_command_path(data, cmd);
        if (ret != 0)
        {
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd(":command not foundsosososoosoos\n", 2);
            handle_invalid_command(data, cmd, prev_fd);
            ft_exit_exec(127, data, cmd);
        }
        exec_child(data, cmd, prev_fd);
    }
		
    if (cmd->path)
    {
        free(cmd->path);
        cmd->path = NULL;
    }
    return (pid);
}
