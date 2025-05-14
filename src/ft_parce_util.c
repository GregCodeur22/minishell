/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parce_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 08:47:16 by garside           #+#    #+#             */
/*   Updated: 2025/05/14 17:53:14 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

void  free_cmd_list(t_data *data)
{
  t_cmd *current;
  t_cmd *next;

  if (!data || !data->cmd_list) // Vérification de sécurité : data et cmd_list
    return;

  current = data->cmd_list;
  while (current)
  {
    next = current->next; // Sauvegarder le pointeur vers le prochain élément
    if (current->outfile)
      free(current->outfile);
    if (current->infile)
      free(current->infile);
    if (current->args)
      free_split(current->args);
    if (current->heredoc)
      free(current->heredoc);
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
	cmd->heredoc = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	cmd->pipe[0] = -1;
	cmd->pipe[1] = -1;
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
			curr->infile = ft_strdup(token->next->value);
			token = token->next;
		}
		else if (token->type == REDIRECTION_OUT && token->next)
		{
			curr->outfile = ft_strdup(token->next->value);
			curr->append = 0;
			token = token->next;
		}
		else if (token->type == APPEND && token->next)
		{
			curr->outfile = ft_strdup(token->next->value);
			curr->append = 1;
			token = token->next;
		}
		else if (token->type == HEREDOC && token->next)
		{
			curr->heredoc = ft_strdup(token->next->value);
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
void	print_cmds(t_cmd *cmd)
{
	int i;

	while (cmd)
	{
		printf("---- Commande ----\n");
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

		// Redirection entrée
		if (cmd->infile)
			printf("Redir in  : %s\n", cmd->infile);
		else
			printf("Redir in  : (null)\n");

		// Redirection sortie
		if (cmd->outfile)
			printf("Redir out : %s (append: %d)\n", cmd->outfile, cmd->append);
		else
			printf("Redir out : (null)\n");

		// Heredoc
		if (cmd->heredoc)
			printf("Here-doc  : %s\n", cmd->heredoc);
		else
			printf("Here-doc  : (null)\n");
		cmd = cmd->next;
	}
}

