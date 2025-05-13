/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:46 by garside           #+#    #+#             */
/*   Updated: 2025/05/13 16:58:57 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_n_flag(char *str)
{
    int i;

    if (!str || str[0] != '-' || str[1] != 'n')
        return (0);
    i = 2;
    while (str[i] == 'n')
        i++;
    return (str[i] == '\0');
}

int ft_echo(t_data *data)
{
    t_cmd *cmd;
    int newline = 1;
    int i = 0;

    if (!data->cmd_list || !data->cmd_list->args)
        return (ft_printf("\n"), 0);

    cmd = data->cmd_list;
    while (cmd->args[i] && is_n_flag(cmd->args[i]))
    {
        newline = 0;
        i++;
    }
    while (cmd->args[i])
    {
        ft_printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            ft_printf(" ");
        i++;
    }
    if (newline)
        ft_printf("\n");

    return (0);
}

