#include "../octolib/includes/libft.h"
#include "../includes/minishell.h" 

void	free_token(t_token *head)
{
	t_token *tmp;
	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}

t_token *new_token(char *value, TokenType type)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

t_token  *handle_cmd_or_arg(char *input, int *i)
{
	int start = *i;
	int lenght = 0;
	char *value = NULL;

	while (input[*i] && input[*i] != '|' && input[*i] != '<' && input[*i] != '>' && input[*i] != ' ' && input[*i] != '\t')
		(*i)++;
	lenght = *i - start;
	value = ft_substr(input, start, lenght);
	t_token *token = new_token(value, WORD);
	free(value);
	printf("Token value: '%s', type: %d\n", token->value, token->type);
	return (token);
}

t_token *handle_pipe(int *i)
{
	char *value = ft_strdup("|");
	if (!value)
		return (NULL);
	t_token *token = new_token(value, PIPE);
	printf("Token value: '%s', type: %d\n", token->value, token->type);
	(*i)++;
	free(value);
	return (token);
}

t_token *handle_redirection(char *input, int *i)
{
	t_token *token = NULL;

	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		if (input[*i] == '>' && input[*i + 1] == '>' && input[*i + 2] == '>')
			return (ft_putstr_fd("syntax error near unexpected token `>>'\n", 2), NULL);
		token = new_token(">>", APPEND);
		printf("Token value: '%s', type: %d\n", token->value, token->type);
		(*i) += 2;
	}
	else if (input[*i] == '>')
	{
		token = new_token(">", REDIRECTION_OUT);
		printf("Token value: '%s', type: %d\n", token->value, token->type);
		(*i)++;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		if (input[*i] == '<' && input[*i + 1] == '<' && input[*i + 2] == '<')
			return (ft_putstr_fd("syntax error near unexpected token `newline'\n", 2), NULL);
		token = new_token("<<", HEREDOC);
		printf("Token value: '%s', type: %d\n", token->value, token->type);
		(*i) += 2;
	}
	else if (input[*i] == '<')
	{
		token = new_token("<", REDIRECTION_IN);
		printf("Token value: '%s', type: %d\n", token->value, token->type);
		(*i)++;
	}
	return (token);
}

t_token *ft_lexer(char *input, int *token_count)
{
	(void)token_count;
	int i = 0;
	t_token *head = NULL;
	t_token *current_token = NULL;
	t_token *last_token = NULL;

	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
			i++;
		else if (input[i] != '|' && input[i] != '<' && input[i] != '>')
			current_token = handle_cmd_or_arg(input, &i);
		else if (input[i] == '|')
			current_token = handle_pipe(&i);
		else if (input[i] == '>' || input[i] == '<')
		{
			current_token = handle_redirection(input, &i);
			if (!current_token)
				return (NULL);
		}
		// else if (input[i] == '\'' || input[i] == '\"')
		// 	current_token = handle_quotes(input[i], &i);
		
		if (!head)
			head = current_token;
		else
			last_token->next = current_token;
		last_token = current_token;

		// add_token_to_list(&head, current_token);
	}
	return (head);
}