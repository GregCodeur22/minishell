/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garside <garside@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:13:50 by garside           #+#    #+#             */
/*   Updated: 2025/05/24 14:43:46 by garside          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../octolib/includes/libft.h"
#include "../includes/minishell.h" 

t_env	*env_new(char *name, char *value)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	new_env->name = ft_strdup(name);
	if (value)
		new_env->content = ft_strdup(value);
	else
		new_env->content = NULL;
	new_env->next = NULL;
	new_env->prev = NULL;
	return (new_env);
}

void	free_env_list(t_env *new_list)
{
	t_env	*temp;

	while (new_list)
	{
		temp = new_list;
		new_list = new_list->next;
		free(temp->name);
		if (temp->content)
			free(temp->content);
		free(temp);
	}
}

t_token	*get_next_token(t_data *data, int *i)
{
	if (data->input[*i] == '>' || data->input[*i] == '<')
		return (handle_redirection(data->input, i));
	if (data->input[*i] == '|')
		return (handle_pipe(i));
	return (handle_cmd_or_arg(data, i));
}

t_token	*ft_lexer(t_data *data)
{
	int		i;
	t_token	*head;
	t_token	*last;
	t_token	*current;

	i = 0;
	head = NULL;
	last = NULL;
	while (data->input[i])
	{
		skip_whitespace(data->input, &i);
		if (!data->input[i])
			break ;
		current = get_next_token(data, &i);
		if (!current)
			return (NULL);
		add_token_to_list(&head, &last, current);
	}
	return (head);
}
void	print_tokens(t_data * data)
{
	while (data->token)
	{
		printf("token value: %s type %d\n", data->token->value, data->token->type);
		data->token = data->token->next;
	}
}

int valid_parse(t_data *data)
{
	t_token *tmp = data->token;

	while (tmp)
	{
		// Si un opérateur est en fin de ligne sans argument
		if ((tmp->type == PIPE || tmp->type == REDIRECTION_IN || tmp->type == REDIRECTION_OUT
			|| tmp->type == APPEND || tmp->type == HEREDOC)
			&& (!tmp->next || tmp->next->type != WORD))
		{
			printf("minishell: syntax error near unexpected token `newline`\n");
			data->last_status = 2;
			return (1);
		}

		// Si deux opérateurs se suivent sans mot entre eux
		if ((tmp->type == PIPE || tmp->type == REDIRECTION_IN || tmp->type == REDIRECTION_OUT
			|| tmp->type == APPEND || tmp->type == HEREDOC)
			&& tmp->next && (tmp->next->type == PIPE || tmp->next->type == REDIRECTION_IN
			|| tmp->next->type == REDIRECTION_OUT || tmp->next->type == APPEND
			|| tmp->next->type == HEREDOC))
		{
			printf("minishell: syntax error near unexpected token `%s`\n", tmp->next->value);
			data->last_status = 2;
			return (1);
		}

		tmp = tmp->next;
	}
	return (0);
}


int	parse(t_data *data)
{
	t_token	*first;

	if (!data->input)
		return (1);
	data->token = ft_lexer(data);
	if (!data->token)
		return (1);
	first = data->token;
	if (data->token)
	{
		// Fais plutôt :
		if (first->type == PIPE)
		{
			ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
			return (1);
		}
	}
	while (first && first->next)
		first = first->next;
	if (first && first->type == 1)
	{
		data->last_status = 2;
		printf("minishell: syntax error near unexpected token `|`\n");
		return (1);
	}
	//  print_tokens(data);
	data->cmd_list = parse_tokens(data);
	if (!data->cmd_list)
		return (1);
	if (valid_parse(data) == 1)
		return (1);		
	print_cmds(data->cmd_list);
	return (0);
}

