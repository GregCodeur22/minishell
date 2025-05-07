/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:05:04 by garside           #+#    #+#             */
/*   Updated: 2025/05/07 12:30:08 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	contains_pipe(t_token *token)
{
	while (token)
	{
		if (token->type == 1)
			return (0);
		token = token->next;
	}
	return (1);
}

int	contains_redirections(t_token *token)
{
	while (token)
	{
		if (token->type == 2 || token->type == 3
			|| token->type == 4 || token->type == 5)
		{
			return (1);
		}
		token = token->next;
	}
	return (0);
}
