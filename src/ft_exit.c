/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:11:06 by garside           #+#    #+#             */
/*   Updated: 2025/04/18 12:57:26 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int ft_exit(t_data *data)
// {
// 	int code = 0;
// 	if (ft_isalpha(data->token->next->value))
// 	{
// 		ft_putstr_fd("exit: ", 2);
// 		ft_putstr_fd(data->token->next->value, 2);
// 		ft_putstr_fd(": numeric argument required", 2);
// 		return (1);
// 	}
// 	else if ((data->token->next->value >= 0 && 
// 			data->token->value <= 9))
// 	{
// 		//quitter minishell 
// 		// donner au code d'erreur la valeur de data->token->next->value
// 	}
//}