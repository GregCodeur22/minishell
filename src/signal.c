/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:45:06 by garside           #+#    #+#             */
/*   Updated: 2025/05/27 15:11:59 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../octolib/includes/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>

volatile sig_atomic_t g_sigint_received = 0;

void handle_sigint(int sig)
{
	(void)sig;
	if (rl_done == 0) // readline actif
	{
		g_status = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else // hors readline (exécution)
	{
		g_status = 2;
		write(1, "\n", 1);
	}
}


void	init_signal(void)
{
	signal(SIGINT, handle_sigint); // Ctrl+C annule la ligne en shell
	signal(SIGQUIT, SIG_IGN);      // Ctrl+\ ignoré en shell
}

void	reset_signals_child(void)
{
	signal(SIGINT, SIG_DFL);  // Ctrl+C interrompt la commande
	signal(SIGQUIT, SIG_DFL); // Ctrl+\ interrompt la commande
}
