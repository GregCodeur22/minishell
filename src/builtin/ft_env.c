/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:35:45 by garside           #+#    #+#             */
/*   Updated: 2025/06/11 19:13:08 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_data *data)
{
	t_env	*i;

	i = data->env;
	while (i)
	{
		ft_printf("%s=%s\n", i->name, i->content);
		i = i->next;
	}
	return (0);
}
