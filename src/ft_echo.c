/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:46 by garside           #+#    #+#             */
/*   Updated: 2025/04/30 13:56:36 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

static void	print_args(t_token *token)
{
	while (token)
	{
		ft_printf("%s", token->value);
		if (token->next)
			ft_printf(" ");
		token = token->next;
	}
}

int	ft_echo(t_data *data)
{
	int		newline;
	t_token	*token;

	if (!data->token->next)
		return (ft_printf("\n"), 1);
	token = data->token->next;
	newline = 1;
	while (token && is_n_flag(token->value))
	{
		newline = 0;
		token = token->next;
	}
	print_args(token);
	if (newline)
		ft_printf("\n");
	return (0);
}
