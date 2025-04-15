/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:38:39 by garside           #+#    #+#             */
/*   Updated: 2025/04/15 18:45:27 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../octolib/includes/libft.h"


int check_quotes(char *input)
{
	int i = 0;
	
	while (input[i])
	{
		if (input[i] == '\'')
		{
			i++;
			while (input[i] && (input[i] != '\''))
				i++;
			if (!input[i])
				return (1);
		}
		if (input[i] == '\"')
		{
			i++;
			while (input[i] && (input[i] != '\"'))
				i++;
			if (!input[i])
				return (1);
		}
		i++;
	}
	return (0);
}

char	*extract_word_double(char *input, int *i)
{
	int	first;
	int	last;
	char *extract;
	
	first = (*i);
	(*i)++;
	while (input[*i] != '\"')
		(*i)++;
	last = (*i);
	extract = ft_substr(input, first + 1, (last - first - 1));
	(*i)++;
	return (extract);
}

char	*extract_word_single(char *input, int *i)
{
	int	first;
	int	last;
	char *extract;
	
	first = (*i);
	(*i)++;
	while (input[*i] != '\'')
		(*i)++;
	last = (*i);
	extract = ft_substr(input, first + 1, (last - first - 1));
	(*i)++;
	return (extract);
}

char	*handle_quotes(char *input, int *i)
{
	char *word= NULL;
	if (input[*i] == '\"')
		word = extract_word_double(input, i);
	else if  (input[*i] == '\'')
		word = extract_word_single(input, i);
	return (word);
}

/*
	handle_quote donc respecter les espaces et les seprateurs
	gestions des guillemts simples et doubles si simple 
	faire attention au separeteur de chemin : dans le path
	un chemin mal forme ou une commande invalide il faut la traiter 
	a l'interieur des quotes ont n enleve pas les espaces 
*/