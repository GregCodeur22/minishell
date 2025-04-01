/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 14:13:38 by garside           #+#    #+#             */
/*   Updated: 2024/10/15 16:04:34 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t				i;
	unsigned char		*conv;

	conv = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		conv[i] = (char)c;
		i++;
	}
	return (conv);
}
