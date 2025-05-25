/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:09:23 by garside           #+#    #+#             */
/*   Updated: 2025/05/25 18:52:28 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int set_fd_cloexec(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFD);
    if (flags == -1)
        return -1;
    flags |= FD_CLOEXEC;
    if (fcntl(fd, F_SETFD, flags) == -1)
        return -1;
    return 0;
}

char	*get_cmd_path(t_data *data, char **cmd)
{
	if (data->token->value[0] == '/')
		return (ft_strdup(cmd[0]));
	return (find_cmd_path(cmd[0], data));
}

int	exec_child_process(t_data *data, t_cmd *cmd, int stdin, int stdout, int prev_fd)
{
	char	**args;
	char	*path;

	args = cmd->args;
	path = get_cmd_path(data, args);
	if (redirect_management(cmd, prev_fd) == -1)
			return CODE_FAIL;
	if (stdin != STDIN_FILENO	)
		close(stdin);
	if (stdout != STDOUT_FILENO)
		close(stdout);
	if (!path)
	{
		ft_putstr_fd(data->token->value, 2);
		ft_putstr_fd(": command not found\n", 2);
		if (data->cmd_list)
			free_cmd_list(data);
		free_data(data);
		// free_split(args);
		exit(127);
	}
	execve(path, args, data->envp);
	ft_putstr_fd("execve failed\n", 2);
	free_cmd_list(data);
	free_data(data);
	free_split(args);
	free(path);
	exit(127);
}

int	ft_shell(t_data *data, t_cmd *cmd, int stdin, int stdout, int prev_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd("fork failed\n", 2), 1);
	if (pid == 0)
		exec_child_process(data, cmd, stdin, stdout, prev_fd);
	waitpid(pid, &status, 0);
	return ((status >> 8) & 0xFF);
}

int	which_command(t_data *data, t_cmd *cmd, int stdin, int stdout, int prev_fd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(data));
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(data));
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(data, cmd, stdin, stdout));
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(data, cmd));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(data));
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(data));
	if (ft_strncmp(cmd->args[0], "./", 2) == 0)
		return (ft_executables(data, cmd, stdin, stdout));
	return (ft_shell(data, cmd, stdin, stdout, prev_fd));
}

int exec_line(t_data *data, t_cmd *cmd)
{
    int prev_fd = -1;
    int status;
    pid_t wpid;
    pid_t last_pid = -1;

    if (cmd->next == NULL)
    {
        // Cas d’une seule commande : gérer les redirections dans le parent
        // => il faut sauvegarder et restaurer stdin/stdout
        int saved_stdin = dup(STDIN_FILENO);
        int saved_stdout = dup(STDOUT_FILENO);
        if (saved_stdin < 0 || saved_stdout < 0)
            return (perror("dup"), CODE_FAIL);
        if (redirect_management(cmd, prev_fd) == -1)
        {
						set_fd_cloexec(saved_stdin);
						set_fd_cloexec(saved_stdout);
            return CODE_FAIL;
        }
        data->last_status = which_command(data, cmd, saved_stdin, saved_stdout, prev_fd);
        // // Restaurer stdin/stdout dans le parent
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
       	close(saved_stdin);
				close(saved_stdout);
        return data->last_status;
    }
    // Cas de plusieurs commandes (pipeline)
    while (cmd)
    {
        if (cmd->next != NULL)
        {
            if (pipe(cmd->pipe_fd) == -1)
            {
                perror("pipe error");
                return 1;
            }
        }
        else
        {
            cmd->pipe_fd[PIPE_READ] = -1;
            cmd->pipe_fd[PIPE_WRITE] = -1;
        }
        last_pid = ft_process(data, cmd, prev_fd, STDIN_FILENO, STDOUT_FILENO);
        if (prev_fd != -1)
            safe_close(prev_fd);
        if (cmd->next != NULL)
				{
            safe_close(cmd->pipe_fd[PIPE_WRITE]);
            prev_fd = cmd->pipe_fd[PIPE_READ];
				}
        else
            prev_fd = -1;
        cmd = cmd->next;
    }
    if (prev_fd != -1)
        safe_close(prev_fd);
    while ((wpid = wait(&status)) > 0)
    {
        if (wpid == last_pid)
            data->last_status = WEXITSTATUS(status);
    }
    if (cmd)
        free_cmd_list(data);
    return data->last_status;
}
