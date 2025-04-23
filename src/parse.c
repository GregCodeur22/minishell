/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:13:50 by garside           #+#    #+#             */
/*   Updated: 2025/04/23 13:28:49 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../octolib/includes/libft.h"
#include "../includes/minishell.h" 

int	parse(t_data *data)
{
	t_token *first;
	if (!data->input)
		return (1);
	data->token = ft_lexer(data);
	if (!data->token)
		return (1);
	first = data->token;
	if (data->token)
	{
		if (first->type == 1 || first->type == 2 || first->type == 3
				|| first->type == 4 || first->type == 5)
		{
			return (1);
		}
	}
	while (first && first->next)
		first = first->next;
	if (first && first->type == 1)
	{
		printf("minishell: syntax error near unexpected token `|`\n");
		return (1);
	}
	return (0);
}
