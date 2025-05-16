/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parce_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 08:47:16 by garside           #+#    #+#             */
/*   Updated: 2025/05/16 02:22:37 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

void free_redir_list(t_redir *redir)
{
    t_redir *tmp;

    while (redir)
    {
        tmp = redir->next;
        if (redir->file)
            free(redir->file);
        free(redir);
        redir = tmp;
    }
}


void  free_cmd_list(t_data *data)
{
  t_cmd *current;
  t_cmd *next;

  if (!data || !data->cmd_list)
    return;

  current = data->cmd_list;
  while (current)
  {
    next = current->next; // Sauvegarder le pointeur vers le prochain élément
		if (current->outfile)
				free_redir_list(current->outfile);
		if (current->infile)
				free_redir_list(current->infile);
    if (current->args)
      free_split(current->args);
    free(current); // Libérer le nœud courant
    current = next;    // Passer au nœud suivant
  }
	data->cmd_list = NULL;
}

t_cmd *new_cmd_node(void)
{
	t_cmd *cmd;
	
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

void add_redir(t_redir **redir_list, char *filename, int type)
{
	t_redir *new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return ;
	new_node->file = strdup(filename);
	new_node->type = type;
	new_node->next = NULL;
	if (*redir_list == NULL)
		*redir_list = new_node;
	else
	{
		t_redir *tmp = *redir_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

t_cmd *parse_tokens(t_data *data)
{
	t_cmd   *head = NULL;
	t_cmd   *curr = NULL;
	t_token *token = data->token;

	while (token)
	{
		if (!curr)
		{
			curr = new_cmd_node();
			if (!head)
				head = curr;
		}
		if (token->type == WORD)
		{
			add_arg(curr, token->value);
		}
		else if (token->type == REDIRECTION_IN && token->next)
		{
				add_redir(&curr->infile, token->next->value, REDIRECTION_IN);
				token = token->next;
		}
		else if (token->type == REDIRECTION_OUT && token->next)
		{
				add_redir(&curr->outfile, token->next->value, REDIRECTION_OUT);
				token = token->next;
		}
		else if (token->type == APPEND && token->next)
		{
				add_redir(&curr->outfile, token->next->value, APPEND);
				token = token->next;
		}
		else if (token->type == HEREDOC && token->next)
		{
				add_redir(&curr->infile, token->next->value, HEREDOC);
				token = token->next;
		}
		else if (token->type == PIPE)
		{
			curr->next = new_cmd_node();
			curr = curr->next;
		}
		token = token->next;
	}
	return (head);
}


//fonction pour debug
void print_redirs(t_redir *redir)
{
    while (redir)
    {
        const char *type_str = NULL;
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

void print_cmds(t_cmd *cmd)
{
    int i;
    while (cmd)
    {
        printf("---- Commande ----\n");
        // Affichage args
        i = 0;
        if (cmd->args)
        {
            printf("Args : ");
            while (cmd->args[i] != NULL)
                printf("[%s] ", cmd->args[i++]);
            printf("\n");
        }
        else
            printf("Args : (aucun)\n");
        // Affichage redirections entrée
        printf("Redirs in  :\n");
        if (cmd->infile)
            print_redirs(cmd->infile);
        else
            printf("  (aucune)\n");
        // Affichage redirections sortie
        printf("Redirs out :\n");
        if (cmd->outfile)
            print_redirs(cmd->outfile);
        else
            printf("  (aucune)\n");
        cmd = cmd->next;
    }
}


