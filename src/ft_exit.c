/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:11:06 by garside           #+#    #+#             */
/*   Updated: 2025/05/13 17:55:27 by garside          ###   ########.fr       */
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
	t_cmd *cmd;
	
	cmd = data->cmd_list;
	if (!cmd->args[1])
	{
		free_data(data);
		free_cmd_list(data);
		ft_printf("exit\n");
		exit(0);
	}
	else if (!ft_isalldigit(cmd->args[1]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_cmd_list(data);
		free_data(data);
		exit(2);
	}
	else if (!cmd->args[1])
		return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
	code = ft_atoi(cmd->args[1]);
	free_data(data);
	free_cmd_list(data);
	ft_printf("exit\n");
	exit (code);
	return (0);
}
