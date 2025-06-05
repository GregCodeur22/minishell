/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broboeuf <broboeuf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:05:40 by garside           #+#    #+#             */
/*   Updated: 2025/06/05 00:21:05 by broboeuf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_is_valid(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	check_name(char *str, t_env *node, char *content)
{
	t_env	*current;

	current = node;
	while (current)
	{
		if (ft_strcmp(current->name, str) == 0)
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

char	*get_content(char *str, int i)
{
	if (str[i] == '=')
		return (ft_substr(str, i + 1, ft_strlen(str) - (i + 1)));
	return (NULL);
}
