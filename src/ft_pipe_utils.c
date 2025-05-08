/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:05:04 by garside           #+#    #+#             */
/*   Updated: 2025/05/08 16:42:50 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_cmd2(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_cmd	*temp;

	tmp = cmd;
	if (cmd != NULL)
	{
		while (tmp)
		{
			if (tmp->args != NULL)
				free_split(tmp->args);
			if (tmp->path != NULL)
				free(tmp->path);
			temp = tmp;
			tmp = tmp->next;
			free(temp);
		}
	}
	return ;
}

int exec_pipeline(t_cmd *cmd_list, t_data *data)
{
    t_cmd *curr;
    int code;

    curr = cmd_list;
    while (curr)
    {
        // Si ce n'est pas le dernier command, créer un pipe
        if (curr->next && pipe(curr->pipefd) == -1)
            return (perror("pipe"), 1);

        curr->pid = fork();
        if (curr->pid == -1)
            return (perror("fork"), 1);
        
        if (curr->pid == 0)
            exec_child(curr, data);  // Passe data à exec_child
        close_parent_pipes(curr);
        curr = curr->next;
    }

    // Attente de tous les processus enfants et récupération du code de sortie
    code = wait_all(cmd_list);
    free_cmd2(cmd_list);
    return (code);
}


void exec_child(t_cmd *cmd, t_data *data)
{
		if (!cmd->path)
		{
				ft_putstr_fd(cmd->cmds[0], 2);
				ft_putstr_fd(": command not found\n", 2);
				free_data(data);
				exit(127);
		}
    // Redirection d'entrée si la commande précédente existe
    if (cmd->prev && dup2(cmd->prev->pipefd[0], 0) == -1)
        exit(1);
    
    // Redirection de sortie si la commande suivante existe
    if (cmd->next && dup2(cmd->pipefd[1], 1) == -1)
        exit(1);
    
    // Fermer les pipes dans le processus enfant
    if (cmd->prev)
        close(cmd->prev->pipefd[0]);
    if (cmd->next)
    {
        close(cmd->pipefd[0]);
        close(cmd->pipefd[1]);
    }
    else if (cmd->pipefd[0] != -1)
        close(cmd->pipefd[0]);

    // Exécution de la commande avec execve
    if (execve(cmd->path, cmd->cmds, NULL) == -1)
    {
        perror("execve failed");
        free_data(data);  // Libération de la mémoire avant la sortie du processus
        exit(127);
    }
}

void	close_parent_pipes(t_cmd *cmd)
{
	if (cmd->prev && cmd->prev->pipefd[0] != -1)
		close(cmd->prev->pipefd[0]);
	if (cmd->pipefd[1] != -1)
		close(cmd->pipefd[1]);
}

int	wait_all(t_cmd *cmd)
{
	int		status;
	int		exit_code;
	t_cmd	*tmp;

	exit_code = 0;
	status = 0;
	tmp = cmd;
	while (tmp)
	{
		if (waitpid(tmp->pid, &status, 0) == -1)
		{
			perror("waitpid");
			g_status = 1;
			tmp = tmp->next;
			continue ;
		}
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = 128 + WTERMSIG(status);
		tmp = tmp->next;
	}
	g_status = exit_code;
	return (exit_code);
}

int	has_pipe(t_token *token)
{
	while (token)
	{
		if (token->type == PIPE)
			return (1);
		token = token->next;
	}
	return (0);
}

char	**collect_args_until_pipe(t_token **token)
{
	char	**args;
	int		count;
	t_token	*tmp;
	int		i;

	count = 0;
	tmp = *token;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == WORD)
			count++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
	{
		g_status = 1;
		return (NULL);
	}
	i = 0;
	while (*token && (*token)->type != PIPE)
	{
		if ((*token)->type == WORD)
		{
			args[i] = ft_strdup((*token)->value);
			if (!args[i++])
			{
				g_status = 1;
				return (args);
			}
		}
		*token = (*token)->next;
	}
	args[i] = NULL;
	return (args);
}

void	init_cmd(t_cmd *cmd, char **args)
{
	cmd->cmds = args;
	cmd->args = args;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->pid = -1;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->pipefd[0] = -1;
	cmd->pipefd[1] = -1;
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (
		ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "unset") == 0
	);
}

t_cmd	*cmd_new(char **args, t_data *data)
{
	t_cmd	*cmd;
	//char	**envp;
	int i = 0;

	printf("Building command:\n");
	while (args[i])
	{
		printf("  arg[%d]: %s\n", i, args[i]);
		i++;
	}

	if (is_builtin(args[0]))
		printf("-> Built-in command detected: %s\n", args[0]);
	else
		printf("-> Not a built-in, checking PATH for: %s\n", args[0]);

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		g_status = 1;
		return (NULL);
	}
	init_cmd(cmd, args);
	//envp = env_to_envp(env);
	cmd->path = find_cmd_path(args[0], data);
	//free_split(envp);
	if (!cmd->path)
	{
		g_status = 127;
		return (cmd);
	}
	cmd->env = data->env;
	return (cmd);
}


void	add_cmd_back(t_cmd **head, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

t_cmd	*build_cmd_list(t_data *data)
{
	t_cmd	*head = NULL;
	t_cmd	*curr = NULL;
	char	**args;
	t_token	*token = data->token;

	while (token)
	{
		args = collect_args_until_pipe(&token);
		if (!args)
			break;
		curr = cmd_new(args, data);
		add_cmd_back(&head, curr);
		if (token && token->type == PIPE)
			token = token->next;
	}
	return head;
}
