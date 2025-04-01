/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 08:48:52 by garside           #+#    #+#             */
/*   Updated: 2024/10/16 09:14:43 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	unsigned char		find;
	const unsigned char	*p;

	i = 0;
	find = (unsigned char)c;
	p = (const unsigned char *)s;
	while (i < n)
	{
		if (p[i] == find)
			return ((void *)(p + i));
		i++;
	}
	return (NULL);
}
