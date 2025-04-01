/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:03:07 by garside           #+#    #+#             */
/*   Updated: 2025/03/04 19:46:52 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_all(char **dest)
{
	if (*dest)
	{
		free(*dest);
		*dest = NULL;
	}
}

char	*no_newline(char **dest)
{
	char	*line;

	line = NULL;
	line = ft_strdup(*dest);
	free_all(dest);
	return (line);
}

char	*split_line(char **dest)
{
	char	*newline_pos;
	char	*line;
	char	*rest;
	int		len;

	newline_pos = ft_strchr(*dest, '\n');
	if (!newline_pos)
		return (no_newline(dest));
	len = newline_pos - *dest + 1;
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, *dest, len);
	line[len] = '\0';
	rest = ft_strdup(newline_pos + 1);
	if (!rest)
	{
		free(line);
		free_all(dest);
		return (NULL);
	}
	free(*dest);
	*dest = rest;
	return (line);
}

char	*read_buffer(int fd, char *dest, char *buffer, ssize_t bytes_read)
{
	char	*temp;

	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free_all(&dest);
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(dest, buffer);
		if (!temp)
		{
			free_all(&dest);
			return (NULL);
		}
		free(dest);
		dest = temp;
		if (ft_strchr(dest, '\n'))
			break ;
	}
	return (dest);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	static char	*dest;
	ssize_t		bytes_read;
	char		*line;

	bytes_read = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	dest = read_buffer(fd, dest, buffer, bytes_read);
	free(buffer);
	if (!dest || *dest == '\0')
	{
		free_all(&dest);
		return (NULL);
	}
	line = split_line(&dest);
	if (!line)
	{
		free_all(&dest);
	}
	return (line);
}
