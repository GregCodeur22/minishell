/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:13:50 by garside           #+#    #+#             */
/*   Updated: 2025/06/11 19:09:39 by abeaufil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*get_next_token(t_data *data, int *i)
{
	if (data->input[*i] == '>' || data->input[*i] == '<')
		return (handle_redirection(data->input, i));
	if (data->input[*i] == '|')
		return (handle_pipe(i));
	return (handle_cmd_or_arg(data, i));
}

int	is_token_ok(t_token *token)
{
	char	*str;
	int		in_single;
	int		in_double;
	int		i;

	str = token->value;
	in_single = 0;
	in_double = 0;
	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '\"' && !in_single)
			in_double = !in_double;
		else if (str[i] == ' ' && !in_single && !in_double)
			return (0);
		i++;
	}
	return (1);
}

static int	is_token_invalid(t_token *current, char *trimmed)
{
	if (!current || is_token_ok(current) || current->quoted)
		return (1);
	if (ft_strchr(current->value, '='))
		return (1);
	trimmed = ft_strtrim(current->value, " \t");
	if (!trimmed)
		return (1);
	if (trimmed[0] == '\0')
	{
		free(trimmed);
		return (1);
	}
	free(trimmed);
	return (0);
}

static t_token	*fill_tokens(char **tab, t_token *new_tokens, t_token *last)
{
	int		i;
	t_token	*new;

	i = 0;
	while (tab[i])
	{
		if (ft_strlen(tab[i]) != 0)
		{
			new = new_token(tab[i], WORD, 0);
			if (!new)
				return (free_split(tab), free_token(new_tokens), NULL);
			add_token_to_list(&new_tokens, &last, new);
		}
		i++;
	}
	return (new_tokens);
}

t_token	*check_token_number(t_token *current, char *trimmed)
{
	t_token	*new_tokens;
	t_token	*result;
	char	**tab;

	new_tokens = NULL;
	if (is_token_invalid(current, trimmed))
		return (current);
	tab = ft_split(current->value, ' ');
	if (!tab)
		return (free_token(current), NULL);
	result = fill_tokens(tab, new_tokens, NULL);
	free_token(current);
	free_split(tab);
	return (result);
}
