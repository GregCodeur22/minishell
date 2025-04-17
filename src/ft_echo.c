/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:46 by garside           #+#    #+#             */
/*   Updated: 2025/04/17 18:50:57 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(t_data *data)
{
	int			nl;
	t_token	*i;

	if (ft_strncmp(data->token->next->value, "-n", 2) == 0 && !data->token->next->value[2])
	{
		nl = 0;
		i = data->token->next->next;
	}
	else
	{
		nl = 1;
		i = data->token->next;
	}
	while (i)
	{
		ft_printf("%s", i->value);
		i = i->next;
		if (i)
			ft_printf(" ");
	}
	if (nl)
		ft_printf("\n");
	return (0);
}
