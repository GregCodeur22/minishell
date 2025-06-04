/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:45:23 by garside           #+#    #+#             */
/*   Updated: 2025/06/03 18:44:00 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_data(t_data *data)
{
	if (!data)
		return ;
	free_env_list(data->env);
	free_env_list(data->export);
	free(data->input);
	free_token(data->token);
}

char	**ft_get_cmd(t_data *data)
{
	t_token		*i;
	char		**cmd;
	int			len;
	int			j;

	len = 0;
	i = data->token;
	while (i && i->type == WORD)
	{
		len++;
		i = i->next;
	}
	cmd = malloc(sizeof(char *) * (len + 1));
	i = data->token;
	j = 0;
	while (i && i->type == WORD)
	{
		cmd[j++] = ft_strdup(i->value);
		i = i->next;
	}
	cmd[j] = NULL;
	return (cmd);
}

void	ft_replace_in_env(t_data *data, char *name, char *value)
{
	t_env	*i;

	i = data->env;
	while (i)
	{
		if (ft_strcmp(i->name, name) == 0)
		{
			free(i->content);
			i->content = ft_strdup(value);
			return ;
		}
		i = i->next;
	}
}
