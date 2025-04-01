/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:55:48 by garside           #+#    #+#             */
/*   Updated: 2024/10/16 17:15:06 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	sign = 1;
	while (nptr[i] == '+' || nptr[i] == '-')
	{
		if ((nptr[i] == '+' || nptr[i] == '-')
			&& (nptr[i + 1] == '+' || nptr[i + 1] == '-'))
			return (0);
		else if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	result = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}
