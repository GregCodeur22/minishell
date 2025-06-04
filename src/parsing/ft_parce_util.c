/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parce_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 08:47:16 by garside           #+#    #+#             */
/*   Updated: 2025/06/04 18:53:17 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		return (free(new_node), -1);
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

int	handle_redirection2(t_cmd *curr, t_token *token, int *skip_next_word)
{
	int	ret;

	ret = 0;
	if (!token->next)
		return (0);
	*skip_next_word = 1;
	if (token->type == REDIRECTION_IN)
		ret = add_redir(&curr->infile, token->next->value, REDIRECTION_IN,
				skip_next_word);
	else if (token->type == REDIRECTION_OUT)
		ret = add_redir(&curr->outfile, token->next->value, REDIRECTION_OUT,
				skip_next_word);
	else if (token->type == APPEND)
		ret = add_redir(&curr->outfile, token->next->value, APPEND,
				skip_next_word);
	else if (token->type == HEREDOC)
		ret = add_redir(&curr->infile, token->next->value, HEREDOC,
				skip_next_word);
	return (ret);
}

int	create_parse(t_token *token, t_cmd **curr, int *skip_next_word)
{
	int	ret;

	ret = 0;
	if (token->type == WORD)
		add_arg(*curr, token->value);
	else if (token->type == PIPE)
	{
		(*curr)->next = new_cmd_node();
		(*curr) = (*curr)->next;
	}
	else
	{
		ret = handle_redirection2(*curr, token, skip_next_word);
		if (ret == -1)
			return (-1);
	}
	return (ret);
}

int	loop_parse(t_token *token, t_cmd **curr, t_cmd **head, int *skip_next_word)
{
	int	ret;

	ret = 0;
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
			return (free_cmd_list2(*head), -1);
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
