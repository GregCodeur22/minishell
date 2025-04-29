/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:31:21 by garside           #+#    #+#             */
/*   Updated: 2025/04/29 19:18:05 by garside          ###   ########.fr       */
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

int	add_in_export(t_data *data, char *str)
{
	int			i;
	char		*name;
	char		*content;
	t_env		*new_export;
	t_env		*new_env;

	i = 0;
	new_export = NULL;
	new_env = NULL;
	while (str[i] && str[i] != '=')
		i++;
	name = ft_substr(str, 0, i);
	if (str[i] == '=')
		content = ft_substr(str, i + 1, ft_strlen(str) - (i + 1));
	else
		content = NULL;
	if (check_name(name, data->export, content))
	{
		if (!check_name(name, data->env, content) && content)
		{
			new_env = env_new(name, content);
			ft_lstadd_back_env(&data->env, new_env);
		}
		free(name);
		if (content)
			free(content);
		return (0);
	}
	new_export = env_new(name, content);
	new_env = env_new(name, content);
	if (content != NULL)
		ft_lstadd_back_env(&data->env, new_env);
	else
		free_env_list(new_env);
	ft_lstadd_back_env(&data->export, new_export);
	free(name);
	if (content)
		free(content);
	return (0);
}

int	ft_export(t_data *data)
{
	t_env		*i;
	t_token		*current;

	i = data->export;
	current = data->token->next;
	if (!data->token->next || data->token->next->type != WORD)
	{
		while (i)
		{
			if (!i->content)
				ft_printf("declare -x %s\n", i->name);
			else
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
			return (1);
		}
		else
			add_in_export(data, current->value);
		current = current->next;
	}
	return (0);
}
