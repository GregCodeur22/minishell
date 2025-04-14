/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:13:50 by garside           #+#    #+#             */
/*   Updated: 2025/04/02 18:56:40 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../octolib/includes/libft.h"
#include "../includes/minishell.h" 

int	parse(t_data *data)
{
	if (check_quotes(data->input) == 1)
		return (ft_putstr_fd("fail quotes", 2), 1);
	data->token = ft_lexer(data->input, &data->token_count);
	
	// check_space_epur();
	// check_separator();
	// check_final();
	free_token(data->token);
	return (0);
}
