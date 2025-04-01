/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:40:30 by garside           #+#    #+#             */
/*   Updated: 2024/10/15 17:32:50 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*p_dest;
	unsigned char	*p_src;

	p_dest = (unsigned char *)dest;
	p_src = (unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	if (p_dest < p_src)
	{
		i = 0;
		while (i < n)
		{
			p_dest[i] = p_src[i];
			i++;
		}
	}
	else
	{
		while (n--)
		{
			p_dest[n] = p_src[n];
		}
	}
	return (dest);
}
