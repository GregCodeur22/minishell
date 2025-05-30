/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:34:56 by abeaufil          #+#    #+#             */
/*   Updated: 2025/05/30 17:22:15 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_redir(t_data *data, t_redir **redir_list, char *filename, int type)
{
	t_redir	*new_node;
	t_redir	*tmp;
	(void)data;
	new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return ;
	if (type == HEREDOC)
	{
		new_node->file = get_here_doc(filename);
		if (!new_node->file)  // erreur ou interruption (Ctrl+C)
        {
            g_status = 130;   // signal d'interruption
			free(new_node->file);
            free(new_node);
            return;
        }
		free(new_node->file);
		free(new_node);
    	return; 
	}
	else
		new_node->file = ft_strdup(filename);
	new_node->type = type;
	new_node->next = NULL;
	if (*redir_list == NULL)
		*redir_list = new_node;
	else
	{
		tmp = *redir_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

void	init_cmd_node(t_cmd **head, t_cmd **curr)
{
	*curr = new_cmd_node();
	if (*head == NULL)
		*head = *curr;
}

void	process_redirection(t_data * data, t_token *token, t_cmd **curr, int *skip)
{
	if (!token->next)
		return ;
	if (token->type == REDIRECTION_IN || token->type == HEREDOC)
		add_redir(data, &(*curr)->infile, token->next->value, token->type);
	else
		add_redir(data, &(*curr)->outfile, token->next->value, token->type);
	*skip = 1;
}

void	handle_token(t_data *data, t_token *token, t_cmd **curr, int *skip)
{
	if (token->type == WORD)
		add_arg(*curr, token->value);
	else if (token->type == REDIRECTION_IN
		|| token->type == REDIRECTION_OUT
		|| token->type == APPEND
		|| token->type == HEREDOC)
		process_redirection(data, token, curr, skip);
	else if (token->type == PIPE)
	{
		(*curr)->next = new_cmd_node();
		*curr = (*curr)->next;
	}
}

t_cmd	*parse_tokens(t_data *data)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_token	*token;
	int		skip;

	head = NULL;
	curr = NULL;
	token = data->token;
	skip = 0;
	while (token)
	{
		if (!curr)
			init_cmd_node(&head, &curr);
		if (skip)
			skip = 0;
		else
			handle_token(data, token, &curr, &skip);
		token = token->next;
	}
	return (head);
}
