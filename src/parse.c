/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:13:50 by garside           #+#    #+#             */
/*   Updated: 2025/04/16 15:37:44 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../octolib/includes/libft.h"
#include "../includes/minishell.h" 

int	parse(t_data *data)
{
	if (!data->input)
		return (1);
	if (check_quotes(data->input) == 1)
		return (ft_putstr_fd("quotes error\n", 2), 1);
	data->token = ft_lexer(data);
	return (0);
}
