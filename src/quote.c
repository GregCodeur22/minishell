#include "../includes/minishell.h"
#include "../octolib/includes/libft.h"

int	check_single_quote(char *input)
{
	int single_quote;
	int i;

	single_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && single_quote == 1)
			single_quote = 0;
		else if (input[i] == '\'')
			single_quote = 1;
		i++;
	}
	if (single_quote == 1)
		return (1);
	return (0);
}

int	check_double_quote(char *input)
{
	int single_quote;
	int i;

	single_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' && single_quote == 1)
			single_quote = 0;
		else if (input[i] == '\"')
			single_quote = 1;
		i++;
	}
	if (single_quote == 1)
		return (1);
	return (0);
}

int check_quotes(char *input)
{
	if (check_single_quote(input) == 1)
		return (ft_putstr_fd("Error\n single quote not close\n", 2),1);
	if (check_double_quote(input) == 1)
		return (ft_putstr_fd("Error\n double quote not close\n", 2),1);
	return (0);
}

/*
	handle_quote donc respecter les espaces et les seprateurs
	gestions des guillemts simples et doubles si simple 
	faire attention au separeteur de chemin : dans le path
	un chemin mal forme ou une commande invalide il faut la traiter 

	a l'interieur des quotes ont n enleve pas les espaces 
	
*/