/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:45:06 by garside           #+#    #+#             */
/*   Updated: 2025/05/30 17:27:53 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../octolib/includes/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>

volatile sig_atomic_t	g_sigint_received = 0;

void handle_sigint_heredoc(int sig)
{
    (void)sig;
    g_status = 130;            // statut ctrl+c
    write(1, "\n", 1);
	rl_replace_line("", 0);
    rl_done = 1;               // force readline à sortir (retourne NULL)
}
void init_signals_heredoc(void)
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handle_sigint_heredoc;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}



void	handle_sigint(int sig)
{
	(void)sig;
	if (rl_done == 0)
	{
		g_status = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
	{
		g_status = 2;
		write(1, "\n", 1);
	}
}

void	init_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	reset_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
