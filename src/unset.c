/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:06:08 by garside           #+#    #+#             */
/*   Updated: 2025/04/27 15:44:47 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_from_list(t_env **list, char *name)
{
	t_env *current;
	t_env *prev;

	current = *list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*list = current->next;
			if (current->next)
				current->next->prev = prev;
			free(current->name);
			free(current->content);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_data *data)
{
	t_token	*current;

	current = data->token->next;
	if (!current)
		return (1);
	while (current)
	{
		remove_from_list(&data->env, current->value);
		remove_from_list(&data->export, current->value);
		current = current->next;
	}
	return (0);
}

