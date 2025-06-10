/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:38:51 by garside           #+#    #+#             */
/*   Updated: 2025/06/10 20:48:50 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_quotes_part(t_data *data, int *i, char *value)
{
	char	*tmp;
	char	*temp;

	tmp = handle_quotes(data, i);
	temp = ft_strjoin(value, tmp);
	free(tmp);
	free(value);
	return (temp);
}

t_token	*handle_cmd_or_arg(t_data *data, int *i)
{
	char	*value;
	t_token	*token;
	int		quoted;

	value = NULL;
	quoted = 0;
	while (is_token_char(data->input[*i]))
	{
		if (data->input[*i] == '$')
			value = handle_env_value(data, i, value);
		else
		{
			value = handle_plain_text(data, i, value);
			if (data->input[*i] == '\'' || data->input[*i] == '\"')
			{
				quoted = 1;
				value = handle_quotes_part(data, i, value);
			}
		}
	}
	token = new_token(value, WORD, quoted);
	free(value);
	return (token);
}


t_token	*handle_double_redir(char *input, int *i)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		if (input[*i + 2] == '>')
		{
			ft_putstr_fd("syntax error near unexpected token `>>'\n", 2);
			return (NULL);
		}
		(*i) += 2;
		return (new_token(">>", APPEND, 0));
	}
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		if (input[*i + 2] == '<')
		{
			ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
			return (NULL);
		}
		(*i) += 2;
		return (new_token("<<", HEREDOC, 0));
	}
	return (NULL);
}

t_token	*handle_redirection(char *input, int *i)
{
	int		count;
	char	type;

	count = 0;
	type = input[*i];
	while (input[*i + count] == type)
		count++;
	if (count > 2)
		return (printf("%s '%c%c'\n", ERR_SYNT, type, type), NULL);
	if (count == 2)
	{
		if (type == '>')
			return (*i += 2, new_token(">>", APPEND, 0));
		else if (type == '<')
			return (*i += 2, new_token("<<", HEREDOC, 0));
	}
	else if (count == 1)
	{
		if (type == '>')
			return (*i += 1, new_token(">", REDIRECTION_OUT, 0));
		else if (type == '<')
			return (*i += 1, new_token("<", REDIRECTION_IN, 0));
	}
	return (NULL);
}

void	skip_whitespace(const char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}
