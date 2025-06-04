/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parce_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 08:47:16 by garside           #+#    #+#             */
/*   Updated: 2025/06/04 17:51:48 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	add_redir(t_redir **redir_list, char *filename, int type,
		int *skip_next_word)
{
	t_redir	*new_node;
	t_redir	*tmp;

	*skip_next_word = 1;
	new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return (1);
	if (type == HEREDOC)
		new_node->file = get_here_doc(filename);
	else
		new_node->file = ft_strdup(filename);
	if (!new_node->file)
	{
		free(new_node);
		return (-1);
	}
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
	return (0);
}

int	create_parse(t_token *token, t_cmd **curr, int *skip_next_word)
{
	int ret = 0;
	if (token->type == WORD)
		add_arg(*curr, token->value);
	else if (token->type == REDIRECTION_IN && token->next)
		add_redir(&(*curr)->infile, token->next->value, REDIRECTION_IN,
			skip_next_word);
	else if (token->type == REDIRECTION_OUT && token->next)
		add_redir(&(*curr)->outfile, token->next->value, REDIRECTION_OUT,
			skip_next_word);
	else if (token->type == APPEND && token->next)
		add_redir(&(*curr)->outfile, token->next->value, APPEND,
			skip_next_word);
	else if (token->type == HEREDOC && token->next)
	{
		ret = add_redir(&(*curr)->infile, token->next->value, HEREDOC,
			skip_next_word);
		if (ret == -1)
			return (-1);		
	}
	else if (token->type == PIPE)
	{
		(*curr)->next = new_cmd_node();
		(*curr) = (*curr)->next;
	}
	return (ret);
}

void	free_cmd_list2(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd;
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
}

int	loop_parse(t_token *token, t_cmd **curr, t_cmd **head,
		int *skip_next_word)
{
	int ret = 0;
	while (token)
	{
		if (!*curr)
		{
			*curr = new_cmd_node();
			if (!*head)
				*head = *curr;
		}
		if (*skip_next_word)
		{
			*skip_next_word = 0;
			token = token->next;
			continue ;
		}
		ret = create_parse(token, curr, skip_next_word);
		if (ret < 0)
		{
			free_cmd_list2(*head);
			return (-1);
		}
		token = token->next;
	}
	return (ret);
}

t_cmd	*parse_tokens(t_data *data)
{
	t_cmd	*head;
	t_cmd	*curr;
	t_token	*token;
	int		skip_next_word;

	head = NULL;
	curr = NULL;
	token = data->token;
	skip_next_word = 0;
	if (loop_parse(token, &curr, &head, &skip_next_word) < 0)
		return (NULL); 
	return (head);
}
