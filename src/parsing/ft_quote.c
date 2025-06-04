/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quote.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:38:39 by garside           #+#    #+#             */
/*   Updated: 2025/06/03 18:44:58 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*append_env_variable(t_data *data, char *extract, int *i, int *first)
{
	int		last;
	char	*teemp;
	char	*tmp;
	char	*temp;

	last = *i;
	teemp = ft_substr(data->input, *first + 1, last - *first - 1);
	tmp = change_env(data, i);
	*first = *i - 1;
	temp = ft_strjoin(teemp, tmp);
	free(teemp);
	free(tmp);
	teemp = ft_strjoin(extract, temp);
	free(temp);
	free(extract);
	return (teemp);
}

char	*append_remaining_segment(t_data *data, char *extract, int first, int i)
{
	char	*tmp;
	char	*temp;

	tmp = ft_substr(data->input, first + 1, i - first - 1);
	temp = ft_strjoin(extract, tmp);
	free(tmp);
	free(extract);
	return (temp);
}

char	*extract_word_double(t_data *data, int *i)
{
	int		first;
	char	*extract;

	extract = NULL;
	first = (*i)++;
	while (data->input[*i] && data->input[*i] != '\"')
	{
		if (data->input[*i] == '$')
		{
			if (data->input[*i + 1] == '?')
				extract = append_error_code(data, extract, i, &first);
			else
				extract = append_env_variable(data, extract, i, &first);
		}
		else
			(*i)++;
	}
	extract = append_remaining_segment(data, extract, first, *i);
	(*i)++;
	return (extract);
}

char	*extract_word_single(char *input, int *i)
{
	int		first;
	int		last;
	char	*extract;

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
	char	*word;

	word = NULL;
	if (data->input[*i] && data->input[*i] == '\"')
		word = extract_word_double(data, i);
	else if (data->input[*i] == '\'')
		word = extract_word_single(data->input, i);
	return (word);
}
