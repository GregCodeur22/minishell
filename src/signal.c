#include "../includes/minishell.h"
#include "../octolib/includes/libft.h" 


void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\nminishell> ", 12);
	rl_on_new_line();
	rl_redisplay();
}

void init_signal(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}