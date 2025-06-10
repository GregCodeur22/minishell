/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:49:50 by garside           #+#    #+#             */
/*   Updated: 2025/06/10 20:42:03 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_one_token(t_token *token)
{
	if (!token->value)
		return ;
	free(token->value);
	if (!token)
		return ;
	free(token);
}

void	free_token(t_token *head)
{
	t_token	*tmp;

	while (head != NULL)
	{
		tmp = head->next;
		if (!head)
			return ;
		free_one_token(head);
		head = tmp;
	}
}

t_token	*new_token(char *value, t_TokenType type, int quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	token->quoted = quoted;
	return (token);
}

char	*handle_error_code(t_data *data, char *value, int *i)
{
	char	*tmp;
	char	*temp;

	(void)data;
	(*i) += 2;
	tmp = ft_itoa(data->last_status);
	temp = ft_strjoin(value, tmp);
	free(tmp);
	if (value)
		free(value);
	value = temp;
	return (value);
}

void	add_token_to_list(t_token **head, t_token **last, t_token *new_token)
{
	if (!*head)
		*head = new_token;
	else
		(*last)->next = new_token;
	*last = new_token;
}
