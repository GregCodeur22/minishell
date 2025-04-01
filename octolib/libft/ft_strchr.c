/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 07:24:58 by garside           #+#    #+#             */
/*   Updated: 2024/10/16 07:41:29 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	find;
	int		i;

	find = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == find)
			return ((char *)(s + i));
		i++;
	}
	if (find == '\0')
		return ((char *)(s + i));
	return (NULL);
}
