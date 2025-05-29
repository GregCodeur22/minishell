/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 08:47:16 by garside           #+#    #+#             */
/*   Updated: 2025/05/29 16:44:32 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

void	free_redir_list(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->type == HEREDOC && redir->file)
			unlink(redir->file);
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = tmp;
	}
}

void	free_cmd_list(t_data *data)
{
	t_cmd	*current;
	t_cmd	*next;

	if (!data || !data->cmd_list)
		return ;
	current = data->cmd_list;
	while (current)
	{
		next = current->next;
		if (current->outfile)
			free_redir_list(current->outfile);
		if (current->infile)
			free_redir_list(current->infile);
		if (current->args)
			free_split(current->args);
		free(current);
		current = next;
	}
	data->cmd_list = NULL;
}

t_cmd	*new_cmd_node(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->path = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

void	add_arg(t_cmd *cmd, char *value)
{
	int		i;
	char	**new_args;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
			i++;
	}
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return ;
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			new_args[i] = ft_strdup(cmd->args[i]);
			i++;
		}
		free_split(cmd->args);
	}
	new_args[i++] = ft_strdup(value);
	new_args[i] = NULL;
	cmd->args = new_args;
}

void	print_redirs(t_redir *redir)
{
	const char	*type_str;

	while (redir)
	{
		type_str = NULL;
		if (redir->type == REDIRECTION_IN)
			type_str = "<";
		else if (redir->type == HEREDOC)
			type_str = "<<";
		else if (redir->type == REDIRECTION_OUT)
			type_str = ">";
		else if (redir->type == APPEND)
			type_str = ">>";
		else
			type_str = "?";
		printf("  %s %s\n", type_str, redir->file);
		redir = redir->next;
	}
}

// void print_cmds(t_cmd *cmd)
// {
// 	int i;
// 	while (cmd)
// 	{
// 		printf("---- Commande ----\n");
// 		i = 0;
// 		if (cmd->args)
// 		{
// 			printf("Args : ");
// 			while (cmd->args[i] != NULL)
// 				printf("[%s] ", cmd->args[i++]);
// 			printf("\n");
// 		}
// 		else
// 			printf("Args : (aucun)\n");
// 		printf("Redirs in  :\n");
// 		if (cmd->infile)
// 			print_redirs(cmd->infile);
// 		else
// 			printf("  (aucune)\n");
// 		printf("Redirs out :\n");
// 		if (cmd->outfile)
// 			print_redirs(cmd->outfile);
// 		else
// 			printf("  (aucune)\n");
// 		cmd = cmd->next;
// 	}
// }
