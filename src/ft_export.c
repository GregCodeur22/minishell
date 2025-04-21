/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:31:21 by garside           #+#    #+#             */
/*   Updated: 2025/04/21 19:39:52 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_is_valid(char *str)
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

t_env *add_in_env(t_data *data, char *str)
{
	char *name;
	char *content;
	t_env *new_node;
	//char **tab_env = NULL;
	int i;
	int first;
	
	i = 0;
	first = i;
	while (str[i] != '=')
		i++;
	name = ft_substr(str, first, i - first);
	if (str[i] == '=')
	{
		first = i + 1;
		while (str[i])
			i++;
		content = ft_substr(str, first, i - first);
	}
	else
		content = NULL;
	new_node = env_new(name, content);
		ft_lstadd_back_env(&data->export, new_node);
	// tab_env = translate_in_tab(data);
	// sort(tab_env);
	// data->export = init_env_list(tab_env);
	free(name);
	if (content)
		free(content);
	return (data->export);
}

int ft_export(t_data *data)
{
	t_env	*i;
	t_token *current;
	
	i = data->export;
	current = data->token->next;
	if (!data->token->next)
	{
		while (i)
		{
			ft_printf("declare -x %s=\"%s\"\n", i->name, i->content);
			i = i->next;
		}
		return (0);
	}
	while (current)
	{
		if (ft_is_valid(current->value) != 0)
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(current->value, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		else
			add_in_env(data, current->value);
		current = current->next;
	}
	return (0);
}
