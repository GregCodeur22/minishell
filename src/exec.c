/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:09:23 by garside           #+#    #+#             */
/*   Updated: 2025/04/17 18:49:57 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_data(t_data *data)
{
	free_env_list(data->env);
	free(data->input);
	free_token(data->token);
}

char	**ft_get_cmd(t_data *data)
{
	t_token	*i;
	char		**cmd;
	int			len;
	int			j;

	len = 0;
	i = data->token;
	while (i && i->type == WORD)
	{
		len++;
		i = i->next;
	}
	cmd = malloc(sizeof(char *) * (len + 1));
	i = data->token;
	j = 0;
	while (i && i->type == WORD)
	{
		cmd[j++] = ft_strdup(i->value);
		i = i->next;
	}
	cmd[j] = NULL;
	return (cmd);
}

int ft_shell(t_data *data)
{
	char	*path;
	char	**cmd;
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (ft_putstr_fd("fork failed\n", 2), 1);
	if (pid == 0)	
	{
		cmd = ft_get_cmd(data);
		if (data->token->value[0] == '/')
			path = ft_strdup(cmd[0]);
		else
			path = find_cmd_path(cmd[0], data->envp);
		if (!path)
		{
			ft_putstr_fd("command not found\n", 2);
			free_data(data);
			free_split(cmd);
			exit(127);
		}
		execve(path, cmd, data->envp);
		ft_putstr_fd("execve failed\n", 2);
		free_data(data);
		free_split(cmd);
		free(path);
		exit(127);
	}
	waitpid(pid, &status, 0);
	return ((status >> 8) & 0xFF);
}

int	which_command(t_data *data)
{
	if (ft_strncmp(data->token->value, "echo", 4) == 0 && !data->token->value[4])
		return (ft_echo(data));
	if (ft_strncmp(data->token->value, "pwd", 3) == 0 && !data->token->value[3])
		return (ft_pwd());
	if (ft_strncmp(data->token->value, "env", 3) == 0 && !data->token->value[3])
		return (ft_env(data));
	if (ft_strncmp(data->token->value, "cd", 2) == 0 && !data->token->value[2])
		return (ft_cd(data));
	if (ft_strncmp(data->token->value, "./", 2) == 0)
		return (ft_executables(data));
	return (ft_shell(data));
}

int exec_line(t_data *data)
{
	return (which_command(data));
}
