/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:35:45 by garside           #+#    #+#             */
/*   Updated: 2025/04/17 18:41:30 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
