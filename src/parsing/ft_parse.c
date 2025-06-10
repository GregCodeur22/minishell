/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abeaufil <abeaufil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:13:50 by garside           #+#    #+#             */
/*   Updated: 2025/06/10 20:51:51 by abeaufil         ###   ########.fr       */
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

t_token	*check_token_number(t_token *current)
{
	t_token	*new_tokens;
	t_token	*last;
	char	**tab;
	int		i;
	char	*trimmed;
	t_token	*new;

	new_tokens = NULL;
	last = NULL;
	i = 0;
	if (!current || is_token_ok(current) || current->quoted)
		return (current);
	if (ft_strchr(current->value, '=') != NULL)
		return (current);
	trimmed = ft_strtrim(current->value, " \t");
	if (!trimmed)
		return (current);
	if (trimmed[0] == '\0')
	{
		free(trimmed);
		return (current);
	}
	free(trimmed);
	tab = ft_split(current->value, ' ');
	if (!tab)
		return (free_token(current), NULL);
	while (tab[i])
	{
		if (ft_strlen(tab[i]) == 0)
		{
			i++;
			continue ;
		}
		new = new_token(tab[i], WORD, 0);
		if (!new)
			return (free_split(tab), free_token(new_tokens), NULL);
		add_token_to_list(&new_tokens, &last, new);
		i++;
	}
	free_token(current);
	free_split(tab);
	return (new_tokens);
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
			return (free_token(head), NULL);
		current = check_token_number(current);
		if (!current)
			return (free_token(head), NULL);
		add_token_to_list(&head, &last, current);
	}
	return (head);
}

void	print_tokens(t_data *data)
{
	t_token	*tmp;

	tmp = data->token;
	while (tmp)
	{
		printf("token value: %s type %d\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
}

int	valid_parse(t_data *data)
{
	t_token	*tmp;

	tmp = data->token;
	while (tmp)
	{
		if (tmp->type != WORD && !tmp->next && tmp->type != PIPE)
		{
			g_status = 2;
			return (printf("%s `newline`\n", ERR_SYNT), 1);
		}
		if (tmp->type == PIPE && tmp->next && tmp->next->type == PIPE)
		{
			g_status = 2;
			return (printf("%s `|`\n", ERR_SYNT), 1);
		}
		if ((tmp->type != WORD && tmp->type != PIPE) && (tmp->next
				&& tmp->next->type != WORD))
		{
			g_status = 2;
			return (printf("%s `%s`\n", ERR_SYNT, tmp->next->value), 1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	parse(t_data *data)
{
	t_token	*token;

	if (!data->input)
		return (1);
	data->token = ft_lexer(data);
	if (!data->token)
		return (1);
	// print_tokens(data);
	if (valid_parse(data) == 1)
		return (1);
	token = data->token;
	if (token->type == PIPE)
		return (printf("%s `|'\n", ERR_SYNT), 1);
	while (token && token->next)
		token = token->next;
	if (token->type == PIPE)
		return (g_status = 2, printf("%s `|'\n", ERR_SYNT), 1);
	data->cmd_list = parse_tokens(data);
	// print_cmd_list(data->cmd_list);
	if (!data->cmd_list)
		return (1);
	if (!data->cmd_list->args && !data->cmd_list->outfile
		&& !data->cmd_list->infile)
		return (1);
	return (0);
}

#include <stdio.h>

// Helpers pour afficher les redirections
void	print_redirs(t_redir *redir, const char *label)
{
	while (redir)
	{
		printf("  %s -> [%s] (type: %d)\n", label, redir->file, redir->type);
		redir = redir->next;
	}
}

// Fonction principale pour afficher une liste de commandes
void	print_cmd_list(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd)
	{
		printf("-------- CMD %d --------\n", i++);
		// Args
		if (cmd->args)
		{
			j = 0;
			while (cmd->args[j])
			{
				printf("  arg[%d]: [%s]\n", j, cmd->args[j]);
				j++;
			}
		}
		else
			printf("  args: (null)\n");
		// Redirs
		if (cmd->infile)
			print_redirs(cmd->infile, "infile");
		if (cmd->outfile)
			print_redirs(cmd->outfile, "outfile");
		// Path
		if (cmd->path)
			printf("  path: [%s]\n", cmd->path);
		else
			printf("  path: (null)\n");
		// Pipe info
		printf("  here_doc_mode: %d\n", cmd->here_doc_mode);
		printf("  saved_stdin: %d\n", cmd->saved_stdin);
		printf("  saved_stdout: %d\n", cmd->saved_stdout);
		cmd = cmd->next;
	}
}
