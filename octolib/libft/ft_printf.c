/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 19:53:01 by garside           #+#    #+#             */
/*   Updated: 2025/03/04 19:47:09 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		total_len;

	i = 0;
	total_len = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			total_len += ft_specifieur(format[i], args);
		}
		else
		{
			write(1, &format[i], 1);
			total_len++;
		}
		i++;
	}
	va_end(args);
	return (total_len);
}
