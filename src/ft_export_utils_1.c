/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:05:40 by garside           #+#    #+#             */
/*   Updated: 2025/04/30 14:07:02 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_is_valid(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (1);
	while (str[i])
	{
		if (ft_isalnum(str[i]) && str[i] == '_')
			return (1);
		i++;
	}
	return (0);
}

int	check_name(char *str, t_env *node, char *content)
{
	t_env	*current;

	current = node;
	while (current)
	{
		if (ft_strcmp (current -> name, str) == 0)
		{
			if (content)
			{
				free(current->content);
				current->content = ft_strdup(content);
				return (1);
			}
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	free_name_content(char *name, char *content)
{
	free(name);
	if (content)
		free(content);
}

char	*get_content(char *str, int i)
{
	if (str[i] == '=')
		return (ft_substr(str, i + 1, ft_strlen(str) - (i + 1)));
	return (NULL);
}
