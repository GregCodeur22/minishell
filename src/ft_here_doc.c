/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_here_doc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:49:33 by garside           #+#    #+#             */
/*   Updated: 2025/05/30 17:28:46 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void    made_new_file(int *fd, char **name)
{
    static int  nb_file = 0;
    char        *nb_str;

    nb_str = ft_itoa(nb_file);
    *name = ft_strjoin("/tmp/here_doc_", nb_str);
    free(nb_str);
    *fd = open(*name, O_RDWR | O_CREAT | O_TRUNC, 0644);
    nb_file++;
}

void fill_here_doc_file(int fd, char *delimiter)
{
    char *line;

    g_status = 0;
    init_signals_heredoc();

    while (1)
    {
		// if (rl_done)
        // 	break ;
        line = readline("> ");
        if (!line || g_status == 130)   // interruption ctrl+c
        {
            free(line);
            break ;
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break ;
        }
        ft_putstr_fd(line, fd);
        ft_putchar_fd('\n', fd);
        free(line);
    }
}

char *get_here_doc(char *str)
{
	char *file_name;
	char *delimiter;
	int here_doc_fd;

	delimiter = ft_strdup(str);
	file_name = NULL;
	made_new_file(&here_doc_fd, &file_name);
	if (here_doc_fd == -1)
	{
		free(delimiter);
		return (ft_printf("error to create a tmp file\n"), NULL);
	}
	fill_here_doc_file(here_doc_fd, delimiter);
	free(delimiter);
	close(here_doc_fd);

	if (g_status == 1)
	{
		unlink(file_name);
		free(file_name);
		return NULL;
	}
	return file_name;
}
