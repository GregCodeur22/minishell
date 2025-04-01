/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 19:08:36 by garside           #+#    #+#             */
/*   Updated: 2025/03/11 18:23:52 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	total_len(unsigned int n)
{
	int	len;

	if (n <= 0)
		len = 1;
	else
		len = 0;
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

int	ft_print_unsigned(unsigned int nb)
{
	char	c;

	if (nb == 0)
	{
		write(1, "0", 1);
		return (1);
	}
	if (nb >= 10)
	{
		ft_print_unsigned(nb / 10);
	}
	c = (nb % 10) + '0';
	write(1, &c, 1);
	return (total_len(nb));
}
