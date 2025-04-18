/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:31:21 by garside           #+#    #+#             */
/*   Updated: 2025/04/18 16:46:41 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_valid_name(char *str)
{
		int i;

		if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		{
				ft_putstr_fd("export: `", 2);
				ft_putstr_fd(str, 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				return (1);
		}
		i = 1;
		while (str[i] && str[i] != '=')
		{
				if (!ft_isalnum(str[i]) && str[i] != '_')
				{
						ft_putstr_fd("export: `", 2);
						ft_putstr_fd(str, 2);
						ft_putstr_fd("': not a valid identifier\n", 2);
						return (1);
				}
				i++;
		}
		return (0);
}

int ft_export(t_data *data)
{
	t_env	*i;
	
	i = data->export;
	if (!data->token->next)
	{
		while (i)
		{
			ft_printf("declare -x %s=\"%s\"\n", i->name, i->content);
			i = i->next;
		}
	}
	// else if (data->token->next->next && data->token->next->next->type == WORD)
	// {
	// 		if (ft_valid_name(data->token->next->next->value) == 0)
	// 		{
				
	// 		}
	// }
	return (0);
}