/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:49:50 by garside           #+#    #+#             */
/*   Updated: 2025/04/29 18:54:38 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

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

t_token	*new_token(char *value, TokenType type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

char	*handle_error_code(t_data *data, char *value, int *i)
{
	char	*tmp;
	char	*temp;

	(*i) += 2;
	tmp = ft_itoa(data->last_status);
	temp = ft_strjoin(value, tmp);
	free(tmp);
	if (value)
		free(value);
	value = temp;
	return (value);
}
