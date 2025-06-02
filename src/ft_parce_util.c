/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parce_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 08:47:16 by garside           #+#    #+#             */
/*   Updated: 2025/06/02 15:27:59 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	add_redir(t_redir **redir_list, char *filename, int type,
		int *skip_next_word)
{
	t_redir	*new_node;
	t_redir	*tmp;

	*skip_next_word = 1;
	new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return ;
	if (type == HEREDOC)
		new_node->file = get_here_doc(filename);
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

void	create_parse(t_token *token, t_cmd **curr, int *skip_next_word)
{
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
		add_redir(&(*curr)->infile, token->next->value, HEREDOC,
			skip_next_word);
	else if (token->type == PIPE)
	{
		(*curr)->next = new_cmd_node();
		(*curr) = (*curr)->next;
	}
}

void	loop_parse(t_token *token, t_cmd **curr, t_cmd **head,
		int *skip_next_word)
{
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
		create_parse(token, curr, skip_next_word);
		token = token->next;
	}
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
	loop_parse(token, &curr, &head, &skip_next_word);
	return (head);
}
