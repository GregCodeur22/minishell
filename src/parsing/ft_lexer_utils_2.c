/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:10:25 by garside           #+#    #+#             */
/*   Updated: 2025/06/10 20:49:00 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_skippable_char(char c)
{
	return (c && c != '|' && c != '<' && c != '>'
		&& c != ' ' && c != '\t' && c != '\'' && c != '\"' && c != '$');
}

int	is_token_char(char c)
{
	return (c && c != '|' && c != '<' && c != '>'
		&& c != ' ' && c != '\t');
}

char	*handle_env_value(t_data *data, int *i, char *value)
{
	char	*tmp;
	char	*temp;

	if (data->input[*i + 1] == '?')
		return (handle_error_code(data, value, i));
	tmp = change_env(data, i);
	temp = ft_strjoin(value, tmp);
	free(tmp);
	free(value);
	return (temp);
}

char	*handle_plain_text(t_data *data, int *i, char *value)
{
	int		start;
	int		len;
	char	*tmp;
	char	*temp;

	start = *i;
	while (is_skippable_char(data->input[*i]))
		(*i)++;
	len = *i - start;
	tmp = ft_substr(data->input, start, len);
	temp = ft_strjoin(value, tmp);
	free(tmp);
	free(value);
	return (temp);
}

t_token	*handle_pipe(int *i)
{
	char		*value;
	t_token		*token;

	value = ft_strdup("|");
	if (!value)
		return (NULL);
	token = new_token(value, PIPE, 0);
	(*i)++;
	free(value);
	return (token);
}
