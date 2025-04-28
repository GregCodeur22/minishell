/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:11:06 by garside           #+#    #+#             */
/*   Updated: 2025/04/28 11:14:09 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isalldigit(char *str)
{
	int	i;
	
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_data *data)
{
	int	code;

	if (!data->token->next || data->token->next->type != WORD)
	{
		free_data(data);
		ft_printf("exit\n");
		exit(0);
	}
	else if (!ft_isalldigit(data->token->next->value))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(data->token->next->value, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_data(data);
		exit(2);
	}
	else if (data->token->next->next && data->token->next->next->type == WORD)
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	code = ft_atoi(data->token->next->value);
	free_data(data);
	ft_printf("exit\n");
	exit (code);
	return (0);
}
