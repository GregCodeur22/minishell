/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:13:50 by garside           #+#    #+#             */
/*   Updated: 2025/04/29 17:46:46 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../octolib/includes/libft.h"
#include "../includes/minishell.h" 

t_env	*env_new(char *name, char *value)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	new_env->name = ft_strdup(name);
	if (value)
		new_env->content = ft_strdup(value);
	else
		new_env->content = NULL;
	new_env->next = NULL;
	new_env->prev = NULL;
	return (new_env);
}

void	free_env_list(t_env *new_list)
{
	t_env	*temp;

	while (new_list)
	{
		temp = new_list;
		new_list = new_list->next;
		free(temp->name);
		if (temp->content)
			free(temp->content);
		free(temp);
	}
}

int	parse(t_data *data)
{
	t_token	*first;

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
			return (1);
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
