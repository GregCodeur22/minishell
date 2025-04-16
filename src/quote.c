/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:38:39 by garside           #+#    #+#             */
/*   Updated: 2025/04/16 18:08:43 by garside          ###   ########.fr       */
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

char	*ft_get_env(char *str, t_data *data)
{
	t_env	*current;
	char	*value;
	int		len;

	current = data->env;
	len = ft_strlen(str);
	while (current)
	{
		if (strncmp(str, current->name, len) == 0)
		{
			value = ft_strdup(current->content);
			return (value);
		}
		current = current->next;
	}
	return (NULL);
}

char	*change_env(t_data *data, int *i)
{
	int first;
	int last;
	char *name;
	char *value;
	
	(*i)++;
	first = (*i);
	while (ft_isalnum(data->input[*i]) || data->input[*i] == '_')
		(*i)++;
	last = (*i);
	name = ft_substr(data->input, first, last - first);
	if (!name[0])
		return (free(name), ft_strdup("$"));
	value = ft_get_env(name, data);
	free(name);
	return (value);
}

char	*extract_word_double(t_data *data, int *i)
{
	int		first;
	int		last;
	char	*extract = NULL;
	char	*tmp = NULL;
	char	*temp = NULL;
	char	*teemp = NULL;
	
	first = (*i);
	(*i)++;
	while (data->input[*i] && data->input[*i] != '\"')
	{
		if (data->input[*i] == '$')
		{
			last = (*i);
			teemp = ft_substr(data->input, first + 1, (last - first - 1));
			tmp = change_env(data, i);
			first = (*i) - 1;
			temp = ft_strjoin(teemp, tmp);
			free(teemp);
			free(tmp);
			teemp = ft_strjoin(extract, temp);  
			free(temp);
			free(extract);
			extract = teemp;
		}
		else
			(*i)++;
	}
	last = (*i);
	tmp = ft_substr(data->input, first + 1, (last - first - 1));
	temp = ft_strjoin(extract, tmp);
	free(tmp);
	free(extract);
	extract = temp;
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

char	*handle_quotes(t_data *data, int *i)
{
	char *word= NULL;
	if (data->input[*i] && data->input[*i] == '\"')
		word = extract_word_double(data, i);
	else if  (data->input[*i] == '\'')
		word = extract_word_single(data->input, i);
	return (word);
}
