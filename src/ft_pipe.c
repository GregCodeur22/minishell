/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:27:48 by garside           #+#    #+#             */
/*   Updated: 2025/05/21 14:58:43 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



void	ft_exit_exec(int code, t_data *data, t_cmd *cmd)
{
	if (cmd && cmd->path)
		free(cmd->path);
	if (data)
	{
		free_cmd_list(data);
		free_data(data);
	}
	exit(code);
}
void safe_close(int fd)
{
    if (fd >= 0)
        close(fd);
}

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

int redirect_management(t_cmd *cmd, int prev_fd)
{
    if (manag_infile(cmd, prev_fd) == 1)
    {
        safe_close(cmd->pipe_fd[0]);
        safe_close(cmd->pipe_fd[1]);
        return 1;
    }
    if (manag_outfile(cmd, cmd->pipe_fd) == 1)
    {
        safe_close(cmd->pipe_fd[0]);
        safe_close(cmd->pipe_fd[1]);
        return 1;
    }
    safe_close(cmd->pipe_fd[0]);
    safe_close(cmd->pipe_fd[1]);
    return 0;
}


int	run_builtin(t_data *data, t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (1); // Pas de commande = erreur

	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(data));
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(data));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(data));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(data));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(data));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(data));

	return (1); // Commande inconnue = erreur
}


void exec_child(t_data *data, t_cmd *cmd, int prev_fd)
{
	reset_signals_child();
	if (redirect_management(cmd, prev_fd) == -1)
		ft_exit_exec(1, data, cmd);
	if (is_builtin(cmd->args[0]))
		ft_exit_exec(run_builtin(data, cmd), data, cmd);
	if (cmd->args[0][0] == '.' || cmd->args[0][0] == '/')
	{
		if (access(cmd->args[0], F_OK) == -1)
			no_such_file_or_directory(cmd->args[0]);
		else if (access(cmd->args[0], X_OK) == -1)
			permission_denied(cmd->args[0]);
		else
			execve(cmd->args[0], cmd->args, data->envp);
		ft_exit_exec(126, data, cmd);
	}
	if (cmd->path)
	{
		if (access(cmd->path, X_OK) == -1)
			permission_denied(cmd->path);
		else
			execve(cmd->path, cmd->args, data->envp);
		error_message(cmd->args[0]);
		ft_exit_exec(126, data, cmd);
	}
	error_message(cmd->args[0]);
	ft_exit_exec(127, data, cmd);
}


int ft_process(t_data *data, t_cmd *cmd, int prev_fd)
{
    pid_t pid;

    // Résolution du chemin si besoin
    if (cmd->args && cmd->args[0] && !is_builtin(cmd->args[0]) &&
        cmd->args[0][0] != '.' && cmd->args[0][0] != '/')
    {
        cmd->path = find_cmd_path(cmd->args[0], data);
        if (!cmd->path)
        {
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            return (127);
        }
    }
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (CODE_FAIL);
    }
    if (pid == 0) // Enfant
			exec_child(data, cmd, prev_fd);
    if (cmd->path)
    {
        free(cmd->path);
        cmd->path = NULL;
    }
    return pid;
}
