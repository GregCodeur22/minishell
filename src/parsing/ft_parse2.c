/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:09:19 by abeaufil          #+#    #+#             */
/*   Updated: 2025/06/11 19:10:00 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_lexer(t_data *data)
{
	int		i;
	t_token	*head;
	t_token	*last;
	t_token	*current;
	char	*trimmed;

	trimmed = NULL;
	i = 0;
	head = NULL;
	last = NULL;
	while (data->input[i])
	{
		skip_whitespace(data->input, &i);
		if (!data->input[i])
			break ;
		current = get_next_token(data, &i);
		if (!current)
			return (free_token(head), NULL);
		current = check_token_number(current, trimmed);
		if (!current)
			return (free_token(head), NULL);
		add_token_to_list(&head, &last, current);
	}
	return (head);
}

void	print_tokens(t_data *data)
{
	t_token	*tmp;

	tmp = data->token;
	while (tmp)
	{
		printf("token value: %s type %d\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
}

int	valid_parse(t_data *data)
{
	t_token	*tmp;

	tmp = data->token;
	while (tmp)
	{
		if (tmp->type != WORD && !tmp->next && tmp->type != PIPE)
		{
			g_status = 2;
			return (printf("%s `newline`\n", ERR_SYNT), 1);
		}
		if (tmp->type == PIPE && tmp->next && tmp->next->type == PIPE)
		{
			g_status = 2;
			return (printf("%s `|`\n", ERR_SYNT), 1);
		}
		if ((tmp->type != WORD && tmp->type != PIPE) && (tmp->next
				&& tmp->next->type != WORD))
		{
			g_status = 2;
			return (printf("%s `%s`\n", ERR_SYNT, tmp->next->value), 1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	parse(t_data *data)
{
	t_token	*token;

	if (!data->input)
		return (1);
	data->token = ft_lexer(data);
	if (!data->token)
		return (1);
	if (valid_parse(data) == 1)
		return (1);
	token = data->token;
	if (token->type == PIPE)
		return (printf("%s `|'\n", ERR_SYNT), 1);
	while (token && token->next)
		token = token->next;
	if (token->type == PIPE)
		return (g_status = 2, printf("%s `|'\n", ERR_SYNT), 1);
	data->cmd_list = parse_tokens(data);
	if (!data->cmd_list)
		return (1);
	if (!data->cmd_list->args && !data->cmd_list->outfile
		&& !data->cmd_list->infile)
		return (1);
	return (0);
}
