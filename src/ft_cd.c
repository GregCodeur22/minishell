/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:24:03 by garside           #+#    #+#             */
/*   Updated: 2025/04/17 18:33:40 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd(t_data *data)
{
	char	*home;

	if (!data->token->next || data->token->next->type != WORD)
	{
		home = ft_get_env("HOME", data);
		if (!home)
			return (ft_putstr_fd("cd: HOME not set\n", 2), 1);
		if (chdir(home) == -1)
			return (ft_putstr_fd("chdir failed\n", 2), 1);
	}
	else if (data->token->next->next && data->token->next->next->type == WORD)
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	else
	{
		if (chdir(data->token->next->value) == -1)
		{
			ft_putstr_fd("cd: ", 2);
			ft_putstr_fd(data->token->next->value, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
	}
	return (0);
}
