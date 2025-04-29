/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:46 by garside           #+#    #+#             */
/*   Updated: 2025/04/29 19:10:20 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(t_data *data)
{
	int		nl;
	int		j;
	t_token	*i;

	if (!data->token->next)
	{
		ft_printf("\n");
		return (1);
	}
	i = data->token->next;
	nl = 1;
	while (i && i->value[0] == '-' && i->value[1] == 'n')
	{
		j = 2;
		while (i->value[j] == 'n')
			j++;
		if (i->value[j] != '\0')
			break ;
		nl = 0;
		i = i->next;
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
