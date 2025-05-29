/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:38:51 by garside           #+#    #+#             */
/*   Updated: 2025/05/29 13:59:29 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../octolib/includes/libft.h"
#include "../includes/minishell.h" 

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

	value = NULL;
	while (is_token_char(data->input[*i]))
	{
		if (data->input[*i] == '$')
			value = handle_env_value(data, i, value);
		else
		{
			value = handle_plain_text(data, i, value);
			if (data->input[*i] == '\'' || data->input[*i] == '\"')
				value = handle_quotes_part(data, i, value);
		}
	}
	token = new_token(value, WORD);
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
		return (new_token(">>", APPEND));
	}
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		if (input[*i + 2] == '<')
		{
			ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
			return (NULL);
		}
		(*i) += 2;
		return (new_token("<<", HEREDOC));
	}
	return (NULL);
}

t_token *handle_redirection(char *input, int *i)
{
	int		count;
	char	type;

	count = 0;
	type = input[*i];
	while (input[*i + count] == type)
		count++;
	if (count > 2)
	{
		printf("%s '%c%c'\n", ERR_SYNT, type, type);
		return (NULL);
	}
	if (count == 2)
	{
		if (type == '>')
		{
			*i += 2;
			return (new_token(">>", APPEND));
		}
		else if (type == '<')
		{
			*i += 2;
			return (new_token("<<", HEREDOC));
		}
	}
	else if (count == 1)
	{
		if (type == '>')
		{
			*i += 1;
			return (new_token(">", REDIRECTION_OUT));
		}
		else if (type == '<')
		{
			*i += 1;
			return (new_token("<", REDIRECTION_IN));
		}
	}
	return (NULL);
}

void	skip_whitespace(const char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}
